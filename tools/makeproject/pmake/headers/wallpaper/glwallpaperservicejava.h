const char GLWALLPAPERSERVICEJAVA_STR[] = 
"/*\n"
" * Copyright 2012 - 2014 Future Interface\n"
" * This software is licensed under the terms of the MIT license.\n"
" *\n"
" */\n"
"/*\n"
" * Copyright (C) 2008 The Android Open Source Project\n"
" *\n"
" * Licensed under the Apache License, Version 2.0 (the \"License\");\n"
" * you may not use this file except in compliance with the License.\n"
" * You may obtain a copy of the License at\n"
" *\n"
" *      http://www.apache.org/licenses/LICENSE-2.0\n"
" *\n"
" * Unless required by applicable law or agreed to in writing, software\n"
" * distributed under the License is distributed on an \"AS IS\" BASIS,\n"
" * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
" * See the License for the specific language governing permissions and\n"
" * limitations under the License.\n"
" */\n"
"\n"
"\n"
"package com.fi.tech;\n"
"\n"
"import java.util.ArrayList;\n"
"import java.util.HashMap;\n"
"import java.util.Iterator;\n"
"\n"
"import javax.microedition.khronos.egl.EGL10;\n"
"import javax.microedition.khronos.egl.EGL11;\n"
"import javax.microedition.khronos.egl.EGLConfig;\n"
"import javax.microedition.khronos.egl.EGLContext;\n"
"import javax.microedition.khronos.egl.EGLDisplay;\n"
"import javax.microedition.khronos.egl.EGLSurface;\n"
"import javax.microedition.khronos.opengles.GL10;\n"
"\n"
"import com.fi.tech.GLWallpaperService.GLEngine;\n"
"import com.fi.tech.%s.Paper3D;\n"
"\n"
"import android.service.wallpaper.WallpaperService;\n"
"import android.util.Log;\n"
"import android.view.MotionEvent;\n"
"import android.view.SurfaceHolder;\n"
"\n"
"// Original code provided by Robert Green\n"
"// http://www.rbgrn.net/content/354-glsurfaceview-adapted-3d-live-wallpapers\n"
"public class GLWallpaperService extends WallpaperService \n"
"{\n"
"    public static final String SHARED_PREFS_NAME = \"GLWallpaperSettings\";\n"
"\n"
"    protected static int mEngineCounter = 0;\n"
"    public GLThread mThread = null;\n"
"        \n"
"    static public EGLConfigChooser mEGLConfigChooser = new ConfigChooser(%s);\n"
"    static public EGLContextFactory mEGLContextFactory = new DefaultContextFactory();\n"
"    static public EGLWindowSurfaceFactory mEGLWindowSurfaceFactory = new DefaultWindowSurfaceFactory();\n"
"            \n"
"    @Override\n"
"    public Engine onCreateEngine() \n"
"    {\n"
"        return new GLEngine();\n"
"    }\n"
"\n"
"    // FIXME: Better to move this function out of GLWallpaperService and put it with other GL helper functions.\n"
"    public static void checkEglError(String prompt, EGL10 egl) \n"
"    {\n"
"        int error;\n"
"        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) \n"
"        {\n"
"            Log.e(\"Paper3D\", String.format(\"%%s: EGL error: 0x%%x\", prompt, error));\n"
"        }\n"
"    }\n"
"\n"
"    public class GLEngine extends Engine\n"
"    {\n"
"        private int mEngineID;\n"
"        private PContext mContext;\n"
"\n"
"        @Override\n"
"        public void onTouchEvent(final MotionEvent event) \n"
"        {\n"
"            final int pointerCount = event.getPointerCount();\n"
"            final int cursorIndex;\n"
"            final int cursorState;\n"
"            final int state;\n"
"\n"
"            int cursorIndexLocal = -1; /* Match all pointers. */\n"
"            int action = event.getAction();\n"
"\n"
"            if (action == MotionEvent.ACTION_DOWN) \n"
"            {\n"
"                cursorState = 0;\n"
"                state = 0;\n"
"            } \n"
"            else if (action == MotionEvent.ACTION_MOVE) \n"
"            {\n"
"                cursorState = 2;\n"
"                state = 1;\n"
"            } \n"
"            else if ((action & MotionEvent.ACTION_POINTER_DOWN) == MotionEvent.ACTION_POINTER_DOWN) \n"
"            {\n"
"                cursorState = 0;\n"
"                cursorIndexLocal = event.findPointerIndex(action & MotionEvent.ACTION_POINTER_ID_MASK);\n"
"                state = 1;\n"
"            } \n"
"            else if ((action & MotionEvent.ACTION_POINTER_UP) == MotionEvent.ACTION_POINTER_UP) \n"
"            {\n"
"                cursorState = 3;\n"
"                cursorIndexLocal = event.findPointerIndex(action & MotionEvent.ACTION_POINTER_ID_MASK);\n"
"                state = 1;\n"
"            } \n"
"            else \n"
"            {\n"
"                cursorState = 3;\n"
"                state = 2;\n"
"            }\n"
"\n"
"            cursorIndex = cursorIndexLocal;\n"
"\n"
"            queueEvent(new Runnable() \n"
"            {\n"
"                public void run() {\n"
"                    if (mContext != null)\n"
"                    {\n"
"                        long touchEvent = mContext.touchEvent(pointerCount, state);\n"
"                        for (int i = 0; i < pointerCount; ++i) \n"
"                        {\n"
"                            int touchState = 1;\n"
"                            if ((cursorIndex < 0) || (cursorIndex == i)) \n"
"                            {\n"
"                                touchState = cursorState;\n"
"                            }\n"
"\n"
"                            mContext.touchCursor(\n"
"                                touchEvent, \n"
"                                i,\n"
"                                event.getPointerId(i), \n"
"                                event.getX(i),\n"
"                                event.getY(i), \n"
"                                event.getPressure(i),\n"
"                                event.getSize(i), \n"
"                                touchState,\n"
"                                event.getEventTime());\n"
"                        }\n"
"                    }\n"
"                }\n"
"            });\n"
"\n"
"            try \n"
"            {\n"
"                Thread.sleep(1);\n"
"            } \n"
"            catch (InterruptedException e) {\n"
"            }\n"
"\n"
"            super.onTouchEvent(event);\n"
"        }\n"
"        \n"
"        public GLEngine() \n"
"        {\n"
"            super();\n"
"            mEngineID = ++mEngineCounter;\n"
"            \n"
"            // The thread is created if it has not. There is only one thread inside\n"
"            // GLWallpaperService instance.\n"
"            if (mThread == null) \n"
"            {\n"
"                mThread = new GLThread();\n"
"                mThread.start();\n"
"            }\n"
"\n"
"            mContext = null;\n"
"        }\n"
"\n"
"        @Override\n"
"        public void onVisibilityChanged(boolean visible) \n"
"        {\n"
"            if (visible) \n"
"            {\n"
"                onResume();\n"
"            } \n"
"            else \n"
"            {\n"
"                onPause();\n"
"            }\n"
"            super.onVisibilityChanged(visible);\n"
"        }\n"
"\n"
"        @Override\n"
"        public void onCreate(SurfaceHolder surfaceHolder) \n"
"        {\n"
"            super.onCreate(surfaceHolder);\n"
"            Log.d(\"GLEngine\"+mEngineID, \"GLEngine.onCreate()\");\n"
"            setTouchEventsEnabled(true);\n"
"        }\n"
"\n"
"        @Override\n"
"        public void onDestroy() \n"
"        {\n"
"            Log.d(\"GLEngine\"+mEngineID, \"GLEngine.onDestroy()\");\n"
"            super.onDestroy();\n"
"        }\n"
"\n"
"        @Override\n"
"        public void onSurfaceChanged(SurfaceHolder holder, int format, int width, int height) \n"
"        {\n"
"            Log.d(\"GLEngine\"+mEngineID, \"onSurfaceChanged()\");\n"
"            mThread.onWindowResize(this, holder, width, height);\n"
"            super.onSurfaceChanged(holder, format, width, height);\n"
"        }\n"
"\n"
"        @Override\n"
"        public void onSurfaceCreated(SurfaceHolder holder) \n"
"        {\n"
"            Log.d(\"GLEngine\"+mEngineID, \"onSurfaceCreated()\");\n"
"            mThread.onSurfaceCreated(holder, this);\n"
"            super.onSurfaceCreated(holder);\n"
"        }\n"
"\n"
"        @Override\n"
"        public void onSurfaceDestroyed(SurfaceHolder holder) \n"
"        {\n"
"            Log.d(\"GLEngine\"+mEngineID, \"onSurfaceDestroyed()\");\n"
"            super.onSurfaceDestroyed(holder);\n"
"        }\n"
"\n"
"        public void onPause() \n"
"        {\n"
"            Log.i(\"Paper3D\", \"GLEngine(\" + mEngineID + \"): onPause()\");\n"
"            mThread.onPause(this);\n"
"        }\n"
"\n"
"        public void onResume() \n"
"        {\n"
"            Log.i(\"Paper3D\", \"GLEngine(\" + mEngineID + \"): onResume()\");\n"
"            mThread.onResume(this);\n"
"        }\n"
"\n"
"        public void queueEvent(Runnable r) \n"
"        {\n"
"            mThread.queueEvent(r);\n"
"        }\n"
"\n"
"        public void setContext(PContext context)\n"
"        {\n"
"            mContext = context;\n"
"        }\n"
"    }\n"
"}\n"
"\n"
"class EngineRenderContext\n"
"{\n"
"    private static EGL10        mEgl;\n"
"    private static EGLDisplay   mEglDisplay;\n"
"    private static EGLConfig    mEglConfig;\n"
"    private static EGLContext   mEglContext;\n"
"    private static PContext     mContext;\n"
"    private EGLSurface          mEglSurface;\n"
"    private SurfaceHolder       mSurf;\n"
"\n"
"    private int                 mWidth;\n"
"    private int                 mHeight;\n"
"\n"
"    private boolean             mActive;\n"
"    private boolean             mPaused;\n"
"    private boolean             mSurfaceChanged;\n"
"    private boolean             mSizeChanged;\n"
"\n"
"    public static void start()\n"
"    {\n"
"        Log.i(\"Paper3D\", \"EngineRenderContext: start()\");\n"
"\n"
"        if (mEglDisplay == null || mEgl == null || mEglContext == null)\n"
"        {\n"
"            mEgl = (EGL10) EGLContext.getEGL();\n"
"            mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);\n"
"\n"
"            if (mEglConfig == null) \n"
"            {\n"
"                int[] version = new int[2];\n"
"                mEgl.eglInitialize(mEglDisplay, version);\n"
"                mEglConfig = GLWallpaperService.mEGLConfigChooser.chooseConfig(mEgl, mEglDisplay);\n"
"            }\n"
"            mEglContext = GLWallpaperService.mEGLContextFactory.createContext(mEgl, mEglDisplay, mEglConfig);\n"
"            if (mEglContext == null || mEglContext == EGL10.EGL_NO_CONTEXT) \n"
"            {\n"
"                throw new RuntimeException(\"Failed to create a GL context.\");\n"
"            }\n"
"\n"
"            mContext = new PContext(Paper3D.CONTEXT_NAME);\n"
"        }\n"
"        else\n"
"        {\n"
"            Log.e(\"Paper3D\", \"EngineRenderContext: Start multiple times.\");\n"
"        }\n"
"    }\n"
"\n"
"    public static void finish()\n"
"    {   \n"
"        Log.i(\"Paper3D\", \"EngineRenderContext::finish()\");\n"
"\n"
"        if (mContext != null)\n"
"        {\n"
"            mContext.uninitialize();\n"
"            mContext = null;\n"
"        }\n"
"\n"
"        if (mEglContext != null)\n"
"        {\n"
"            GLWallpaperService.mEGLContextFactory.destroyContext(mEgl, mEglDisplay, mEglContext);    \n"
"        }\n"
"\n"
"        if (mEglDisplay != null) \n"
"        {\n"
"            mEgl.eglTerminate(mEglDisplay);\n"
"        }\n"
"            \n"
"        mEglDisplay = null;\n"
"        mEgl        = null;\n"
"        mEglConfig  = null;\n"
"        mEglContext = null;\n"
"    }\n"
"\n"
"    public EngineRenderContext()\n"
"    {\n"
"        mEglSurface     = null;\n"
"        mSurf           = null;\n"
"        mActive         = false;\n"
"        mPaused         = false;\n"
"        mSurfaceChanged = false;\n"
"        mSizeChanged    = false;\n"
"        mWidth          = 0;\n"
"        mHeight         = 0;\n"
"    }\n"
"\n"
"    public void changeSurface(SurfaceHolder holder, int width, int height)\n"
"    {\n"
"        if (mActive)\n"
"        {\n"
"            throw new RuntimeException(\"Cannot change surface when render is activity\");\n"
"        }\n"
"\n"
"        if (holder != mSurf)\n"
"        {\n"
"            mSurf = holder;\n"
"            mSurfaceChanged = true;\n"
"        }\n"
"\n"
"        if (mWidth != width || mHeight != height)\n"
"        {\n"
"            mWidth = width;\n"
"            mHeight = height;\n"
"            mSizeChanged = true;\n"
"        }\n"
"\n"
"        destroySurface();\n"
"    }\n"
"\n"
"    public void activate()\n"
"    {    \n"
"        if (mActive)\n"
"        {\n"
"            throw new RuntimeException(\"Activate called when active.\");\n"
"        }\n"
"\n"
"        if (mEglContext == null)\n"
"        {\n"
"            throw new RuntimeException(\"No EGL context is available.\");\n"
"        }\n"
"\n"
"        if (mSurf == null)\n"
"        {\n"
"            throw new RuntimeException(\"No native surface is available.\");\n"
"        }\n"
"\n"
"        // Delay to create context/surface\n"
"        if (mEglSurface == null)\n"
"        {\n"
"            createSurface();\n"
"        }\n"
"        mSurfaceChanged = true;\n"
"\n"
"        if (!mEgl.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext))\n"
"        {\n"
"            throw new RuntimeException(\"eglMakeCurrent failed.\");\n"
"        }\n"
"\n"
"        mActive = true;\n"
"    }\n"
"\n"
"    public void deactivate()\n"
"    {\n"
"        if (!mActive)\n"
"        {\n"
"            throw new RuntimeException(\"Deactive called when inactive.\");\n"
"        }\n"
"\n"
"        if (!mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT))\n"
"        {\n"
"            throw new RuntimeException(\"eglMakeCurrent failed.\");\n"
"        }\n"
"\n"
"        mActive = false;\n"
"    }\n"
"\n"
"    private void createSurface()\n"
"    {\n"
"        if (mActive)\n"
"        {\n"
"            throw new RuntimeException(\"createSurface() called when active.\");\n"
"        }\n"
"\n"
"        if (mEglSurface != null)\n"
"        {\n"
"            throw new RuntimeException(\"createSurface() called when mEglSurface is not null.\");\n"
"        }\n"
"\n"
"        mEglSurface = GLWallpaperService.mEGLWindowSurfaceFactory.createWindowSurface(mEgl, mEglDisplay, mEglConfig, mSurf);\n"
"        if (mEglSurface == null || mEglSurface == EGL10.EGL_NO_SURFACE) \n"
"        {\n"
"            throw new RuntimeException(\"createWindowSurface failed\");\n"
"        }\n"
"    }\n"
"\n"
"    private void destroySurface()\n"
"    {\n"
"        if (mActive)\n"
"        {\n"
"            throw new RuntimeException(\"destroySurface() called when active.\");\n"
"        }\n"
"\n"
"        if (mEglSurface != null)\n"
"        {\n"
"\n"
"            GLWallpaperService.mEGLWindowSurfaceFactory.destroySurface(mEgl, mEglDisplay, mEglSurface);\n"
"            mEglSurface = null;\n"
"        }\n"
"    }\n"
"\n"
"    public void pause()\n"
"    {\n"
"        mPaused = true;\n"
"    }\n"
"\n"
"    public void resume()\n"
"    {\n"
"        mPaused = false;\n"
"    }\n"
"\n"
"    public boolean swap()\n"
"    {\n"
"        if (!mActive)\n"
"        {\n"
"            new RuntimeException(\"swap() called but inactive.\");\n"
"        }\n"
"\n"
"        if (!available())\n"
"        {\n"
"            new RuntimeException(\"swap() called but inavailable.\");\n"
"        }\n"
"\n"
"        mEgl.eglSwapBuffers(mEglDisplay, mEglSurface);\n"
"        return mEgl.eglGetError() != EGL11.EGL_CONTEXT_LOST;\n"
"    }\n"
"\n"
"    public boolean active()\n"
"    {\n"
"        return mActive;\n"
"    }\n"
"\n"
"    public boolean available()\n"
"    {\n"
"        return mSurf != null && !mPaused;\n"
"    }\n"
"                        \n"
"    public void onSurfaceChanged(GL10 gl)\n"
"    {\n"
"        if (!mContext.isInitialized())\n"
"        {\n"
"            if (!mContext.initialize(mWidth, mHeight))\n"
"            {\n"
"                mContext = null;\n"
"                throw new RuntimeException(\"Failed to create a Paper3D context.\");\n"
"            }\n"
"        }\n"
"        mContext.resize(mWidth, mHeight);\n"
"    }\n"
"                    \n"
"    public void onDrawFrame(GL10 gl)\n"
"    {\n"
"        mContext.update();\n"
"    }\n"
"\n"
"    public boolean popSizeChanged()\n"
"    {\n"
"        boolean ret = mSizeChanged;\n"
"        mSizeChanged = false;\n"
"        return ret;\n"
"    }\n"
"\n"
"    public boolean popSurfaceChanged()\n"
"    {\n"
"        boolean ret = mSurfaceChanged;\n"
"        mSurfaceChanged = false;\n"
"        return ret;\n"
"    }\n"
"\n"
"    public boolean popSurfaceOrSizeChanged()\n"
"    {\n"
"        boolean ret = mSizeChanged || mSurfaceChanged;\n"
"        popSizeChanged();\n"
"        popSurfaceChanged();\n"
"\n"
"        return ret;\n"
"    }\n"
"\n"
"    public int surfaceWidth() \n"
"    {\n"
"        return mWidth;\n"
"    }\n"
"\n"
"    public int surfaceHeight() \n"
"    {\n"
"        return mHeight;\n"
"    }\n"
"\n"
"    public PContext getContext()\n"
"    {\n"
"        return mContext;\n"
"    }\n"
"}\n"
"\n"
"class GLThread extends Thread \n"
"{\n"
"    private ArrayList<Runnable> mEventQueue = new ArrayList<Runnable>();\n"
"    private ArrayList<Runnable> mCommandQueue = new ArrayList<Runnable>();\n"
"    private static int MAX_EVENT_COUNT_PER_UPDATE = 100;\n"
"    \n"
"    private HashMap<GLEngine, EngineRenderContext> renderContexts = new HashMap<GLEngine, EngineRenderContext>();\n"
"    private EngineRenderContext activeRenderContext = null;\n"
"    \n"
"    private final GLThreadManager sGLThreadManager = new GLThreadManager();\n"
"    private GLThread mEglOwner;\n"
"\n"
"    // Once the thread is started, all accesses to the following member\n"
"    // variables are protected by the sGLThreadManager monitor\n"
"    public boolean mDone;\n"
"    private boolean mEventsWaiting;\n"
"    private boolean mCommandsWaiting;\n"
"\n"
"    GLThread()\n"
"    {\n"
"        super();\n"
"        mDone = false;\n"
"    }\n"
"    \n"
"    @Override\n"
"    public void run() \n"
"    {\n"
"        setName(\"GLThread \" + getId());\n"
"\n"
"        try \n"
"        {\n"
"            guardedRun();\n"
"        } \n"
"        catch (InterruptedException e) \n"
"        {\n"
"            // fall thru and exit normally\n"
"        } \n"
"    }\n"
"\n"
"    private void guardedRun() throws InterruptedException \n"
"    {\n"
"        EngineRenderContext.start();\n"
"\n"
"        try \n"
"        {\n"
"            GL10 gl = null;\n"
"            \n"
"            // This is our main activity thread's loop, we go until asked to quit.\n"
"            while (!isDone()) \n"
"            {\n"
"                // FIXME: eventsWaiting and commandsWaiting are not needed here as it might block the event handling.\n"
"                boolean eventsWaiting = true;\n"
"                boolean commandsWaiting  = true;\n"
"                \n"
"                synchronized (sGLThreadManager) \n"
"                {\n"
"                    eventsWaiting = mEventsWaiting;\n"
"                    commandsWaiting = mCommandsWaiting;\n"
"                }\n"
"\n"
"                if(true)\n"
"                {\n"
"                    Runnable r = null;\n"
"                    while ((r = getCommand()) != null)\n"
"                    {\n"
"                        r.run();\n"
"                    }\n"
"                }\n"
"                \n"
"                // Try activate an engine.\n"
"                // FIXME: why is render context now unrelated to Engine.\n"
"                if (activeRenderContext == null)\n"
"                {\n"
"                    Iterator<EngineRenderContext> iter = renderContexts.values().iterator();\n"
"                    EngineRenderContext renderContext;\n"
"                    while (iter.hasNext()) \n"
"                    {\n"
"                        renderContext = iter.next();\n"
"                        if (renderContext.available())\n"
"                        {\n"
"                            activateContext(renderContext);\n"
"                            break;\n"
"                        }\n"
"                    } \n"
"                }\n"
"\n"
"                if (PNativeLibrary.getInstance().isInitialized()) \n"
"                {\n"
"                    Runnable r = null;\n"
"                    \n"
"                    int eventCounter = 0;\n"
"                    while ((r = getEvent()) != null)\n"
"                    {\n"
"                    	if (eventCounter == 0)\n"
"                        {\n"
"                            Log.i(\"Paper3D\", \"GLThread: Process event ...\");\n"
"                        }\n"
"                        try \n"
"                        { \n"
"                            r.run(); \n"
"                        } \n"
"                        catch (Exception e) \n"
"                        {\n"
"                        }\n"
"                        ++eventCounter;\n"
"                        // Limit the event processing time in order to avoid\n"
"                        // blocking other incoming events.\n"
"                        if (eventCounter >= MAX_EVENT_COUNT_PER_UPDATE) \n"
"                        {\n"
"                            break;\n"
"                        }\n"
"                    }\n"
"\n"
"                    if (eventCounter > 0)\n"
"                    {\n"
"                        Log.i(\"Paper3D\", \"GLThread: Event was processed, count: \" + eventCounter);\n"
"                    }\n"
"                }\n"
"                \n"
"                if (activeRenderContext != null)\n"
"                {\n"
"                    if (activeRenderContext.popSurfaceOrSizeChanged())\n"
"                    {\n"
"                    	Log.i(\"Paper3D\", \"GLThread: Surface has been changed.\");\n"
"                        activeRenderContext.onSurfaceChanged(gl);\n"
"                    }\n"
"                    \n"
"                    activeRenderContext.onDrawFrame(gl);\n"
"                    \n"
"                    activeRenderContext.swap();\n"
"\n"
"                    Thread.sleep(10);\n"
"                }\n"
"                else \n"
"                {\n"
"                    synchronized (sGLThreadManager) \n"
"                    {\n"
"                        sGLThreadManager.wait();\n"
"                    }\n"
"                }\n"
"            }\n"
"        } \n"
"        finally \n"
"        {\n"
"            synchronized (sGLThreadManager) \n"
"            {\n"
"                EngineRenderContext.finish();\n"
"            }\n"
"        }\n"
"    }\n"
"\n"
"    private boolean isDone() \n"
"    {\n"
"        return mDone;\n"
"    }\n"
"\n"
"    private EngineRenderContext getEngineRenderContext(GLEngine engine)\n"
"    {\n"
"        EngineRenderContext renderContext;\n"
"        if (renderContexts.containsKey(engine))\n"
"        {\n"
"            renderContext = (EngineRenderContext)renderContexts.get(engine);\n"
"        }\n"
"        else\n"
"        {\n"
"            renderContext = new EngineRenderContext();\n"
"            engine.setContext(renderContext.getContext());\n"
"            renderContexts.put(engine, renderContext);\n"
"        }\n"
"        return renderContext;\n"
"    }\n"
"    \n"
"    private void activateContext(EngineRenderContext context)\n"
"    {\n"
"        deactiveCurrentContext();\n"
"        activeRenderContext = context;\n"
"        activeRenderContext.activate();\n"
"    }\n"
"    \n"
"    private void deactiveCurrentContext()\n"
"    {\n"
"        if (activeRenderContext != null)\n"
"        {\n"
"            activeRenderContext.deactivate();\n"
"        }\n"
"        activeRenderContext = null;\n"
"    }\n"
"    \n"
"    public void onSurfaceCreated(final SurfaceHolder holder, final GLEngine engine) \n"
"    {\n"
"        Log.i(\"Paper3D\", \"GLThread: surfaceCreated()\");\n"
"        \n"
"        queueCommand( new Runnable() \n"
"        {\n"
"            @Override\n"
"            public void run() \n"
"            {\n"
"                Log.i(\"Paper3D\", \"GLThread: SurfaceCreated on \" + engine);\n"
"                EngineRenderContext context = getEngineRenderContext(engine);\n"
"                if (context.active()) \n"
"                {\n"
"                    deactiveCurrentContext();\n"
"                }\n"
"                context.changeSurface(holder, holder.getSurfaceFrame().width(), holder.getSurfaceFrame().height());\n"
"            }\n"
"        });\n"
"        \n"
"        synchronized(sGLThreadManager)\n"
"        {\n"
"            sGLThreadManager.notifyAll();\n"
"        }\n"
"    }\n"
"\n"
"    public void onWindowResize(final GLEngine engine, final SurfaceHolder surf, final int width, final int height) \n"
"    {\n"
"        Log.i(\"Paper3D\", \"GLThread: onWindowResize()\");\n"
"        \n"
"        queueCommand( new Runnable() \n"
"        {\n"
"            @Override\n"
"            public void run() \n"
"            {\n"
"                Log.i(\"Paper3D\", \"GLThread: Size Changed on \" + engine);\n"
"                EngineRenderContext context = getEngineRenderContext(engine);\n"
"                if (context.active()) \n"
"                {\n"
"                    deactiveCurrentContext();\n"
"                }\n"
"                context.changeSurface(surf, width, height);\n"
"            }\n"
"        });\n"
"        \n"
"        synchronized(sGLThreadManager)\n"
"        {\n"
"            sGLThreadManager.notifyAll();\n"
"        }\n"
"    }\n"
"    \n"
"    public void surfaceDestroyed(final GLEngine engine) {\n"
"        Log.i(\"Paper3D\", \"GLThread: surfaceDestroyed()\");\n"
"        \n"
"        queueCommand( new Runnable() \n"
"        {\n"
"            @Override\n"
"            public void run() \n"
"            {\n"
"                Log.i(\"Paper3D\", \"GLThread: SurfaceDestroyed on \" + engine);\n"
"                EngineRenderContext context = getEngineRenderContext(engine);\n"
"                if(context.active()) \n"
"                {\n"
"                    deactiveCurrentContext();\n"
"                }\n"
"                context.changeSurface(null, 0, 0);\n"
"            }\n"
"        });\n"
"        \n"
"        synchronized(sGLThreadManager)\n"
"        {\n"
"            sGLThreadManager.notifyAll();\n"
"        }\n"
"    }\n"
"\n"
"    public void onPause(final GLEngine engine) \n"
"    {\n"
"        Log.i(\"Paper3D\", \"GLThread: onPause()\");\n"
"        \n"
"        queueCommand( new Runnable() \n"
"        {\n"
"            @Override\n"
"            public void run() \n"
"            {\n"
"                Log.i(\"Paper3D\", \"GLThread: Pause on \" + engine);\n"
"                EngineRenderContext context = getEngineRenderContext(engine);\n"
"                if(context.active()) {\n"
"                    deactiveCurrentContext();\n"
"                }\n"
"                context.pause();\n"
"            }\n"
"        });\n"
"        \n"
"        synchronized(sGLThreadManager)\n"
"        {\n"
"            sGLThreadManager.notifyAll();\n"
"        }\n"
"    }\n"
"\n"
"    public void onResume(final GLEngine engine) \n"
"    {\n"
"        Log.i(\"Paper3D\", \"GLThread: onResume()\");\n"
"        \n"
"        queueCommand( new Runnable() {\n"
"            @Override\n"
"            public void run() \n"
"            {\n"
"                Log.i(\"Paper3D\", \"GLThread: Resume on \" + engine);\n"
"                EngineRenderContext context = getEngineRenderContext(engine);\n"
"                context.resume();\n"
"            }\n"
"        });\n"
"        \n"
"        synchronized(sGLThreadManager)\n"
"        {\n"
"            sGLThreadManager.notifyAll();\n"
"        }\n"
"    }\n"
"\n"
"    public void requestExitAndWait() \n"
"    {\n"
"        Log.i(\"Paper3D\", \"GLThread: requestExitAndWait()\");\n"
"        synchronized (sGLThreadManager) \n"
"        {\n"
"            mDone = true;\n"
"            sGLThreadManager.notifyAll();\n"
"        }\n"
"        \n"
"        try \n"
"        {\n"
"            join();\n"
"        } \n"
"        catch (InterruptedException ex) \n"
"        {\n"
"            Thread.currentThread().interrupt();\n"
"        }\n"
"    }\n"
"\n"
"    public void queueEvent(Runnable r) \n"
"    {\n"
"        synchronized (this) \n"
"        {\n"
"            mEventQueue.add(r);\n"
"\n"
"            synchronized (sGLThreadManager) \n"
"            {\n"
"                mEventsWaiting = true;\n"
"                sGLThreadManager.notifyAll();\n"
"            }\n"
"        }\n"
"    }\n"
"    \n"
"    public void queueCommand(Runnable r)\n"
"    {\n"
"        synchronized (this) \n"
"        {\n"
"            mCommandQueue.add(r);\n"
"\n"
"            synchronized (sGLThreadManager) \n"
"            {\n"
"                mCommandsWaiting = true;\n"
"                sGLThreadManager.notifyAll();\n"
"            }\n"
"        }\n"
"    }\n"
"    \n"
"    private Runnable getCommand() \n"
"    {\n"
"        synchronized (this) \n"
"        {\n"
"            if (mCommandQueue.size() > 0) \n"
"            {\n"
"                return mCommandQueue.remove(0);\n"
"            }\n"
"        }\n"
"        return null;\n"
"    }\n"
"    \n"
"    private Runnable getEvent() \n"
"    {\n"
"        synchronized (this) \n"
"        {\n"
"            if (mEventQueue.size() > 0) \n"
"            {\n"
"                return mEventQueue.remove(0);\n"
"            }\n"
"        }\n"
"        return null;\n"
"    }\n"
"\n"
"    private class GLThreadManager \n"
"    {\n"
"    }\n"
"}\n"
"\n"
"interface EGLConfigChooser \n"
"{\n"
"    EGLConfig chooseConfig(EGL10 egl, EGLDisplay display);\n"
"}\n"
"\n"
"class ConfigChooser implements EGLConfigChooser \n"
"{\n"
"    private int[] configAttribs;\n"
"\n"
"    /**\n"
"     * @param multisamples\n"
"     */\n"
"    public ConfigChooser(int multisamples) \n"
"    {\n"
"        if (multisamples > 1) \n"
"        {\n"
"            int[] conf = { EGL10.EGL_SAMPLES, multisamples, EGL10.EGL_SAMPLE_BUFFERS,\n"
"                1, EGL10.EGL_RED_SIZE, EGL10.EGL_DONT_CARE,\n"
"                EGL10.EGL_GREEN_SIZE, EGL10.EGL_DONT_CARE,\n"
"                EGL10.EGL_BLUE_SIZE, EGL10.EGL_DONT_CARE,\n"
"                EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_RENDERABLE_TYPE, 4,\n"
"                EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };\n"
"            configAttribs = conf;\n"
"        } \n"
"        else \n"
"        {\n"
"            int[] conf = { EGL10.EGL_SAMPLES, 0, EGL10.EGL_SAMPLE_BUFFERS,\n"
"                0, EGL10.EGL_RED_SIZE, EGL10.EGL_DONT_CARE,\n"
"                EGL10.EGL_GREEN_SIZE, EGL10.EGL_DONT_CARE,\n"
"                EGL10.EGL_BLUE_SIZE, EGL10.EGL_DONT_CARE,\n"
"                EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_RENDERABLE_TYPE, 4,\n"
"                EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };\n"
"            configAttribs = conf;\n"
"        }\n"
"    }\n"
"\n"
"    /**\n"
"     * @param egl\n"
"     * @param display\n"
"     * @return\n"
"     */\n"
"    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {\n"
"        int[] num_config = new int[1];\n"
"        EGLConfig[] config = new EGLConfig[1];\n"
"        egl.eglChooseConfig(display, configAttribs, config, 1, num_config);\n"
"\n"
"        return config[0];\n"
"    }\n"
"}\n"
"\n"
"// ////////////////////////////////////////////////////////////////////////////////////////////////////////\n"
"\n"
"abstract class BaseConfigChooser implements EGLConfigChooser \n"
"{\n"
"    public BaseConfigChooser(int[] configSpec) \n"
"    {\n"
"        mConfigSpec = configSpec;\n"
"    }\n"
"\n"
"    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) \n"
"    {\n"
"        int[] num_config = new int[1];\n"
"        egl.eglChooseConfig(display, mConfigSpec, null, 0, num_config);\n"
"\n"
"        int numConfigs = num_config[0];\n"
"\n"
"        if (numConfigs <= 0) {\n"
"            throw new IllegalArgumentException(\"No configs match configSpec\");\n"
"        }\n"
"\n"
"        EGLConfig[] configs = new EGLConfig[numConfigs];\n"
"        egl.eglChooseConfig(display, mConfigSpec, configs, numConfigs,\n"
"                num_config);\n"
"        EGLConfig config = chooseConfig(egl, display, configs);\n"
"        if (config == null) \n"
"        {\n"
"            throw new IllegalArgumentException(\"No config chosen\");\n"
"        }\n"
"        return config;\n"
"    }\n"
"\n"
"    abstract EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs);\n"
"\n"
"    protected int[] mConfigSpec;\n"
"\n"
"    public static class ComponentSizeChooser extends BaseConfigChooser \n"
"    {\n"
"        public ComponentSizeChooser(int redSize, int greenSize, int blueSize,\n"
"                int alphaSize, int depthSize, int stencilSize) \n"
"        {\n"
"            super(new int[] \n"
"                    {\n"
"                        EGL10.EGL_RED_SIZE, redSize,\n"
"                        EGL10.EGL_GREEN_SIZE, greenSize, EGL10.EGL_BLUE_SIZE,\n"
"                        blueSize, EGL10.EGL_ALPHA_SIZE, alphaSize,\n"
"                        EGL10.EGL_DEPTH_SIZE, depthSize, EGL10.EGL_STENCIL_SIZE,\n"
"                        stencilSize, EGL10.EGL_NONE \n"
"                    });\n"
"\n"
"            mValue = new int[1];\n"
"            mRedSize = redSize;\n"
"            mGreenSize = greenSize;\n"
"            mBlueSize = blueSize;\n"
"            mAlphaSize = alphaSize;\n"
"            mDepthSize = depthSize;\n"
"            mStencilSize = stencilSize;\n"
"        }\n"
"\n"
"        @Override\n"
"        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) \n"
"        {\n"
"            EGLConfig closestConfig = null;\n"
"            int closestDistance = 1000;\n"
"            for (EGLConfig config : configs) \n"
"            {\n"
"                int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);\n"
"                int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);\n"
"                if (d >= mDepthSize && s >= mStencilSize) \n"
"                {\n"
"                    int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);\n"
"                    int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);\n"
"                    int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);\n"
"                    int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);\n"
"\n"
"                    int distance = Math.abs(r - mRedSize)\n"
"                        + Math.abs(g - mGreenSize)\n"
"                        + Math.abs(b - mBlueSize)\n"
"                        + Math.abs(a - mAlphaSize);\n"
"\n"
"                    if (distance < closestDistance) \n"
"                    {\n"
"                        closestDistance = distance;\n"
"                        closestConfig = config;\n"
"                    }\n"
"                }\n"
"            }\n"
"            return closestConfig;\n"
"        }\n"
"\n"
"        private int findConfigAttrib(EGL10 egl, EGLDisplay display,\n"
"                EGLConfig config, int attribute, int defaultValue) \n"
"        {\n"
"\n"
"            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) \n"
"            {\n"
"                return mValue[0];\n"
"            }\n"
"            return defaultValue;\n"
"        }\n"
"\n"
"        private int[] mValue;\n"
"        // Subclasses can adjust these values:\n"
"        protected int mRedSize;\n"
"        protected int mGreenSize;\n"
"        protected int mBlueSize;\n"
"        protected int mAlphaSize;\n"
"        protected int mDepthSize;\n"
"        protected int mStencilSize;\n"
"    }\n"
"\n"
"    /**\n"
"     * This class will choose a supported surface as close to RGB565 as\n"
"     * possible, with or without a depth buffer.\n"
"     * \n"
"     */\n"
"    public static class SimpleEGLConfigChooser extends ComponentSizeChooser \n"
"    {\n"
"        public SimpleEGLConfigChooser(boolean withDepthBuffer) \n"
"        {\n"
"            super(4, 4, 4, 0, withDepthBuffer ? 16 : 0, 0);\n"
"            // Adjust target values. This way we'll accept a 4444 or\n"
"            // 555 buffer if there's no 565 buffer available.\n"
"            mRedSize = 5;\n"
"            mGreenSize = 6;\n"
"            mBlueSize = 5;\n"
"        }\n"
"    }\n"
"}\n"
"\n"
"/**\n"
" * An interface for customizing the eglCreateWindowSurface and eglDestroySurface\n"
" * calls.\n"
" * \n"
" * \n"
" * This interface must be implemented by clients wishing to call\n"
" * {@link GLWallpaperService#setEGLWindowSurfaceFactory(EGLWindowSurfaceFactory)}\n"
" */\n"
"interface EGLWindowSurfaceFactory \n"
"{\n"
"    EGLSurface createWindowSurface(EGL10 egl, EGLDisplay display,\n"
"            EGLConfig config, Object nativeWindow);\n"
"\n"
"    void destroySurface(EGL10 egl, EGLDisplay display, EGLSurface surface);\n"
"}\n"
"\n"
"class DefaultWindowSurfaceFactory implements EGLWindowSurfaceFactory \n"
"{\n"
"    public EGLSurface createWindowSurface(EGL10 egl, EGLDisplay display,\n"
"            EGLConfig config, Object nativeWindow) \n"
"    {\n"
"        // this is a bit of a hack to work around Droid init problems - if you\n"
"        // don't have this, it'll get hung up on orientation changes\n"
"        EGLSurface eglSurface = null;\n"
"        while (eglSurface == null) \n"
"        {\n"
"            try \n"
"            {\n"
"                eglSurface = egl.eglCreateWindowSurface(display, config, nativeWindow, null);\n"
"            } \n"
"            catch (Throwable t) \n"
"            {\n"
"            } \n"
"            finally \n"
"            {\n"
"                if (eglSurface == null) \n"
"                {\n"
"                    try \n"
"                    {\n"
"                        Thread.sleep(10);\n"
"                    } \n"
"                    catch (InterruptedException t) \n"
"                    {\n"
"                    }\n"
"                }\n"
"            }\n"
"        }\n"
"        Log.i(\"Paper3D\", \"GLThread: Surface created \" + eglSurface.toString() + \" on \" + nativeWindow.toString() );\n"
"        return eglSurface;\n"
"    }\n"
"\n"
"    public void destroySurface(EGL10 egl, EGLDisplay display, EGLSurface surface) \n"
"    {\n"
"        Log.i(\"Paper3D\", \"GLThread: Destory surface \" + surface.toString());\n"
"        egl.eglDestroySurface(display, surface);\n"
"    }\n"
"}\n"
"\n"
"/**\n"
" * An interface for customizing the eglCreateContext and eglDestroyContext\n"
" * calls.\n"
" * \n"
" * \n"
" * This interface must be implemented by clients wishing to call\n"
" * {@link GLWallpaperService#setEGLContextFactory(EGLContextFactory)}\n"
" */\n"
"interface EGLContextFactory \n"
"{\n"
"    EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig);\n"
"\n"
"    void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context);\n"
"}\n"
"\n"
"class DefaultContextFactory implements EGLContextFactory \n"
"{\n"
"    private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;\n"
"\n"
"    public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig config) \n"
"    {\n"
"        Log.i(\"Paper3D\", \"GLThread: creating OpenGL ES 2.0 context\");\n"
"        int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };\n"
"        EGLContext context = egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, attrib_list);\n"
"        return context;\n"
"    }\n"
"\n"
"    public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) \n"
"    {\n"
"        egl.eglDestroyContext(display, context);\n"
"    }\n"
"}\n"
;
