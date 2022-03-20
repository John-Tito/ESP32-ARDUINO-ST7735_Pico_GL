#include "picotk.h"
#include "debugger.h"
#include "screen_config.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/internal/zgl.h>
#include <MemfbDefs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *FrameBuffer = (void *)NULL;
static FBDevGLXContext *ctx;
int (*m_init)(void), (*m_update)(void);
/* new window size or exposure */
void reshape(int width, int height)
{
    GLfloat h = (GLfloat)height / (GLfloat)width;

    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -40.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

struct TinyFBDevGLXContext
{
    GLContext *gl_context; /* base class */
    void *screen;
    ZBuffer *zb; // 绘制用buffer
    int line_length;
};

/**
 * 初始化
 */
static void initialize_fbdev()
{

    // 按照屏幕大小创建FrameBuffer空间
    FrameBuffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BITS_PER_PIXEL / 8);

    if (FrameBuffer == NULL)
    {
        fprintf(stderr, "error: unable create framebuffer: %s\n",
                strerror(errno));
        exit(1);
    }

    fmt->println("info: succ create framebuffer");
}

static void shutdown_fbdev(void)
{
    // 释放FrameBuffer
    free(FrameBuffer);
}

void errorcatcher(); /* routine to handle errors */

void tkSwapBuffers(void)
{
    fbdev_glXSwapBuffers(ctx);
}

int ui_init(int (*init)(void), int (*update)(void))
{
    m_init = init;
    m_update = update;

    ctx = fbdev_glXCreateContext();

    if (ctx == NULL)
    {
        return -1;
    }

    initialize_fbdev();

    fbdev_glXMakeCurrent(ctx);

    fmt->printf("debug:tk.cpp: succ create cxt at %x, gl_context at %x\n", (unsigned int)ctx, (unsigned int)(ctx->gl_context));

    fmt->printf("debug:tk.cpp: succ create zbuf{%d,%d,%d,%d} at %x\n", ctx->gl_context->zb->xsize, ctx->gl_context->zb->ysize, ctx->gl_context->zb->linesize, ctx->gl_context->zb->mode, (unsigned int)(ctx->gl_context->zb));

    // scene_init();
    (*m_init)();

    fmt->printf("debug:tk.cpp: toggle reshaping (%d,%d)\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    reshape(SCREEN_WIDTH, SCREEN_HEIGHT);

    fmt->printf("debug:tk.cpp: reshaped (%d,%d), start render loop\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    return 0;
}

unsigned long last_time = 0, current_time = 0, frame_cnt = 0, sum_time = 0, d_time = 0;

void ui_loop(void)
{
    d_time = millis();
    m_update();
    d_time = millis() - d_time;
    sum_time += d_time;
    frame_cnt += 1;
    current_time = millis();
    if ((current_time - last_time) >= 4999)
    {
        last_time = current_time;
        Serial.printf("fr: %d,c: %d\n", (int)frame_cnt / 5, (int)(sum_time / frame_cnt));
        frame_cnt = 0;
        sum_time = 0;
    }
}

int ui_end()
{
    shutdown_fbdev();
    return 0;
}

/*
 * Here on an unrecoverable error.
 *
 */
void errorcatcher()
{
}

/**
 * 获得帧缓存
 * @return
 */
void *getFrameBuffer()
{
    return FrameBuffer;
}

size_t sizeOfFrameBuffer()
{
    return (size_t)SCREEN_HEIGHT * SCREEN_LINE_BYTES_LENGTH;
}
