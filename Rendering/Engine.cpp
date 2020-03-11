//
// Created by Jake Meegan on 03/03/2020.
//

#include "Engine.h"
#include "../Animation/BVH.h"

Engine::Engine(QWidget *parent) : QOpenGLWidget(parent), camera({0, 4, 10}), bvh("walking.bvh") {
}

void Engine::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST); // enable z-buffering
    // set lighting parameters
//    glShadeModel(GL_FLAT);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL); // enable colour

    // set background colour to purple
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

    startLoop(); // start game loop
}

void Engine::startLoop() {
    // start elapsed timer
    elapsed_timer.start();

    // start game loop
    connect(&timer, &QTimer::timeout, this, QOverload<>::of(&Engine::loop));
    timer.start();
}

void Engine::loop() {
    // get time from previous frame
    GLfloat frame_time = elapsed_timer.nsecsElapsed() * 0.000000001;
    elapsed_timer.restart();
    delta_accumulator += frame_time;

    camera.update();

    // integrate in steps
    while (delta_accumulator >= DELTA_TIME) {
        delta_accumulator -= DELTA_TIME;
    }

    // catch remainder in accumulator and integrate
    if(delta_accumulator >= 0.0f) {

        delta_accumulator = 0.0f;
    }


    frame += frame_time / bvh.interval;
    if(frame >= bvh.num_frame)
        frame = 0;

    if(input.keyboard.KEY_W)
        camera.zoom(frame_time);
    if(input.keyboard.KEY_S)
        camera.zoom(-frame_time);
    if(input.keyboard.KEY_A)
        camera.pan(frame_time, 0);
    if(input.keyboard.KEY_D)
        camera.pan(-frame_time, 0);


    // call window/opengl to update
    update();
}

void Engine::resizeGL(int w, int h) {
    const float aspectRatio = (float) w / (float) h;

    // resize viewport to window size
    glViewport(0, 0, w, h);

    // set projection matrix to glFrustum based on window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-aspectRatio, aspectRatio, -1.0, 1.0, 1, 100.0);
}

void Engine::paintGL() {
    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // enable lighting
    glEnable(GL_LIGHTING);

    // set model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(&camera.getView()[0][0]);

    /*
     * Draw objects
     */

    bvh.RenderFigure(frame, 0.5f);


    GLfloat lightPos[] = {0, 30.0f, -10.0f, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // swap buffer
    glutSwapBuffers();
}

void Engine::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_W)
        input.keyboard.KEY_W = true;
    else if(event->key() == Qt::Key_A)
        input.keyboard.KEY_A = true;
    else if(event->key() == Qt::Key_S)
        input.keyboard.KEY_S = true;
    else if(event->key() == Qt::Key_D)
        input.keyboard.KEY_D = true;
}

void Engine::keyReleaseEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_W)
        input.keyboard.KEY_W = false;
    else if(event->key() == Qt::Key_A)
        input.keyboard.KEY_A = false;
    else if(event->key() == Qt::Key_S)
        input.keyboard.KEY_S = false;
    else if(event->key() == Qt::Key_D)
        input.keyboard.KEY_D = false;
}

void Engine::mouseMoveEvent(QMouseEvent *event) {
    float sensitivity = 0.01;
    if (event->buttons() & Qt::LeftButton) { // left click to control rotation
        float xoffset = event->x() - last_m_pos.x();
        float yoffset = event->y() - last_m_pos.y();

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.rotate(xoffset, yoffset);
    }

    last_m_pos = event->pos(); // store last mouse position
}

void Engine::mousePressEvent(QMouseEvent *event) {
    last_m_pos = event->pos(); // clear mouse position upon click
}