#ifdef __APPLE__
#include <backend/mac/CocoaWindow.h>
#include <backend/mac/SpruceView.h>
#include <backend/mac/opengl/OpenGLView.h>
#include <backend/mac/opengl/CocoaOpenGLContext.h>
#include <backend/mac/metal/MetalView.h>
#include <graphics/renderer/api/metal/MetalContext.h>
#include <backend/mac/objcpp.h>
#include <log.h>

@implementation CocoaWindowObj
- (BOOL) canBecomeKeyWindow {
	return YES;
}

- (BOOL) canBecomeMainWindow {
	return YES;
}
@end

@implementation WindowDelegate
- (instancetype) initWithWindow:(spruce::Window*)_spruceWindow cocoaWindow:(NSWindow*)_cocoaWindow {
	spruceWindow = _spruceWindow;
	cocoaWindow = (CocoaWindowObj*) _cocoaWindow;
	return self;
}

- (void) windowDidResize:(NSNotification*)notification {
	[cocoaWindow update];
	NSRect viewFrame = cocoaWindow.contentView.frame;
	NSRect viewFrameBacking = [cocoaWindow.contentView convertRectToBacking:viewFrame];
	spruceWindow->width = static_cast<uint32>(viewFrameBacking.size.width);
	spruceWindow->height = static_cast<uint32>(viewFrameBacking.size.height);
}

- (void)windowWillClose:(NSNotification *)notification {
	spruceWindow->open = false;
}
@end

namespace spruce {
	CocoaWindow::CocoaWindow() {
		width = 1920;
		height = 1080;
		NSString* appName = @"spruce";
		NSUInteger windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
		NSRect windowRect = NSMakeRect(0, 0, width, height);
		window = [[CocoaWindowObj alloc] initWithContentRect:windowRect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO];
		windowController = [[NSWindowController alloc] initWithWindow:window];
		[window setTitle:appName];
		view = [[NSView alloc] initWithFrame:windowRect];
		[window setContentView:view];
		[window makeFirstResponder:view];
		spruce::Window* spruceWindow = this;
		delegate = [[WindowDelegate alloc] initWithWindow:spruceWindow cocoaWindow:window];
		[window setDelegate:delegate];
		[window makeKeyAndOrderFront: window];
	}

	CocoaWindow::~CocoaWindow() {
		[delegate dealloc];
		delegate = nullptr;
		[view release];
		view = nullptr;
		[windowController release];
		windowController = nullptr;
		[window release];
		window = nullptr;
	}

	[[nodiscard]] owner<APIContext> CocoaWindow::initAPI(app::API api) noexcept {
		NSRect viewRect = NSMakeRect(0, 0, width, height);
		NSView* oldView = window.contentView;
		APIContext* context;
		switch (api) {
			case app::OPENGL:
				this->view = [[OpenGLView alloc] initWithFrame:viewRect window:this];
				[window setContentView:this->view];
				[window makeFirstResponder:this->view];
				[(OpenGLView*)view initContext];
				[[(OpenGLView*)view getContext] makeCurrentContext];
				context = new CocoaOpenGLContext((OpenGLView*)this->view);
				break;
			case app::METAL:
				this->view = [[MetalView alloc] initWithFrame:viewRect window:this];
				[window setContentView:this->view];
				[window makeFirstResponder:this->view];
				context = new MetalContext(this->view);
				break;
			default:
				context = nullptr;
				serr("unsupported api");
				break;
		}
		[oldView release];
		[window update];
		if (api == app::OPENGL) {
			[window.contentView update];
		}
		return context;
	}

	void CocoaWindow::setTitle(string title) noexcept {
		NSString* nTitle = convertStr(title);
		[window setTitle:nTitle];
	}

	void CocoaWindow::setVisible(bool visible) noexcept {
		[window setOpaque:visible];
	}

	void CocoaWindow::setFullscreen(bool fullscreen) noexcept {
		if ([view isInFullScreenMode]) {
			if (!fullscreen) {
				[view exitFullScreenModeWithOptions:nil];
			}
		} else {
			if (fullscreen) {
				[view enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];
			}
		}
	}

	void CocoaWindow::close() noexcept {
		[windowController close];
		open = false;
	}

	void CocoaWindow::setCursorMode(input::CursorMode mode) noexcept {
		this->cursorMode = mode;
		if (mode == input::HIDDEN || mode == input::DISABLED) {
			[NSCursor hide];
		} else {
			[NSCursor unhide];
		}
	}
}
#endif
