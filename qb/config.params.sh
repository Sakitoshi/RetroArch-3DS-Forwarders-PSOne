HAVE_LIBRETRODB=yes        # Enable libretrodb
HAVE_RGUI=yes              # Enable RGUI menu
HAVE_MATERIALUI=auto       # Enable MaterialUI menu 
HAVE_CHEEVOS=no            # Enable Cheevos (achievements)
HAVE_XMB=auto              # Enable XMB menu
HAVE_ZAHNRAD=no            # Enable Zahnrad menu
HAVE_DYNAMIC=yes           # Enable dynamic loading of libretro library
HAVE_SDL=auto              # SDL support
HAVE_SDL2=auto             # SDL2 support (disables SDL 1.x)
C89_SDL2=no
HAVE_LIBUSB=auto           # Libusb HID support
C89_LIBUSB=no
HAVE_UDEV=auto             # Udev/Evdev gamepad support
HAVE_LIBRETRO=             # libretro library used
HAVE_MAN_DIR=              # Manpage install directory
HAVE_GLES_LIBS=            # Link flags for custom GLES library
HAVE_GLES_CFLAGS=          # C-flags for custom GLES library
HAVE_THREADS=auto          # Threading support
HAVE_FFMPEG=auto           # Enable FFmpeg recording support
C89_FFMPEG=no
HAVE_SSA=auto              # Enable SSA/ASS for FFmpeg subtitle support
HAVE_DYLIB=auto            # Enable dynamic loading support
HAVE_NETWORKING=auto       # Enable networking features (recommended)
HAVE_NETPLAY=auto          # Enable netplay support (requires networking)
HAVE_D3D9=yes              # Enable Direct3D 9 support
HAVE_OPENGL=auto           # Enable OpenGL support
HAVE_GLES=no               # Use GLESv2 instead of desktop GL
HAVE_MALI_FBDEV=no         # Enable Mali fbdev context support
HAVE_VIVANTE_FBDEV=no      # Enable Vivante fbdev context support
HAVE_OPENDINGUX_FBDEV=no   # Enable Opendingux fbdev context support
HAVE_GLES3=no              # Enable OpenGLES3 support
HAVE_X11=auto              # Enable everything X11.
HAVE_OMAP=no               # Enable OMAP video support
HAVE_XINERAMA=auto         # Enable Xinerama support.
HAVE_KMS=auto              # Enable KMS context support
HAVE_EXYNOS=no             # Enable Exynos video support
HAVE_DISPMANX=no           # Enable Dispmanx video support
HAVE_SUNXI=no              # Enable Sunxi video support
HAVE_WAYLAND=auto          # Enable Wayland support
C89_WAYLAND=no
HAVE_EGL=auto              # Enable EGL context support
HAVE_VG=auto               # Enable OpenVG support
HAVE_CG=auto               # Enable Cg shader support
HAVE_LIBXML2=auto          # Enable libxml2 support
HAVE_ZLIB=auto             # Enable zlib support (ZIP extract, PNG decoding/encoding)
HAVE_FBO=auto              # Enable render-to-texture (FBO) support
HAVE_ALSA=auto             # Enable ALSA support
C89_ALSA=no
HAVE_OSS=auto              # Enable OSS support
HAVE_RSOUND=auto           # Enable RSound support
HAVE_ROAR=auto             # Enable RoarAudio support
HAVE_AL=auto               # Enable OpenAL support
HAVE_JACK=auto             # Enable JACK support
HAVE_COREAUDIO=auto        # Enable CoreAudio support
HAVE_PULSE=auto            # Enable PulseAudio support
C89_PULSE=no
HAVE_FREETYPE=auto         # Enable FreeType support
HAVE_STB_FONT=yes          # Enable stb_truetype font support
HAVE_XVIDEO=auto           # Enable XVideo support
HAVE_PYTHON=auto           # Enable Python 3 support for shaders
C89_PYTHON=no
HAVE_V4L2=auto             # Enable video4linux2 support
HAVE_NEON=no               # Forcefully enable ARM NEON optimizations
HAVE_SSE=no                # Forcefully enable x86 SSE optimizations (SSE, SSE2)
HAVE_FLOATHARD=no          # Force hard float ABI (for ARM)
HAVE_FLOATSOFTFP=no        # Force soft float ABI (for ARM)
HAVE_7ZIP=yes              # Compile in 7z support
HAVE_PRESERVE_DYLIB=no     # Enable dlclose() for Valgrind support
HAVE_PARPORT=auto          # Parallel port joypad support
HAVE_IMAGEVIEWER=yes       # Built-in image viewer support.
C89_IMAGEVIEWER=no         # stb_image hates C89
HAVE_MMAP=auto             # MMAP support
HAVE_QT=no                 # QT companion support
HAVE_XSHM=no               # XShm video driver support (disabled because it's just a dummied out stub)
HAVE_CHEEVOS=yes           # Enable Retro Achievements
HAVE_VULKAN=auto           # Enable Vulkan support
C89_VULKAN=no