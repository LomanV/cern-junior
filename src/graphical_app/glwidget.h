#pragma once

#include <QOpenGLWidget>
#include <QTime>
#include "opengl_view.h"
#include "content.h"

class GLWidget : public QOpenGLWidget{
	public:
		GLWidget(QWidget* parent) : QOpenGLWidget(parent), content(&view){}
		virtual ~GLWidget(){}

	private:
		virtual void initializeGL()                  override;
		virtual void resizeGL(int width, int height) override;
		virtual void paintGL()                       override;

		// Event handling
		virtual void keyPressEvent(QKeyEvent* event) override;
		virtual void timerEvent(QTimerEvent*)        override;

		bool isPaused = true;

		// internal handling
		void pause();

		// viewpoint
		OpenGLView view;

		// timer
		int timerId;
		QTime stopwatch;

		// to be drawn
		Content content;
};
