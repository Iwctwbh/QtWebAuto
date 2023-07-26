#pragma once
#ifndef EVENT_EATER_HPP
#define EVENT_EATER_HPP

#include <QObject>
#include <QEvent>
#include <QMouseEvent>

#include "test_case.h"

class EventEater final : public QObject
{
	Q_OBJECT

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;
};

inline bool EventEater::eventFilter(QObject* obj, QEvent* event)
{
	switch (event->type())
	{
	case QEvent::MouseButtonPress:
	{
		if (!TestCase::CheckIsEvent())
		{
			const QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
			TestCase::Log("Mouse pressed " + QString::number(mouse_event->pos().x()) + "," + QString::number(mouse_event->pos().y()), TestCase::kEvent);
			if (TestCase::is_record_)
			{
				TestCaseStep case_step{ TestCase::GetCaseStep(TestCase::current_url_) };
				case_step.InsertNonAutomatic("@{COORDINATE, " + QString::number(mouse_event->pos().x()) + ", " + QString::number(mouse_event->pos().y()) + "}@");
				TestCase::InsertCaseStep(TestCase::current_url_, case_step);
				TestCase::SaveRecord();
			}
		}
		return false;
	}

	case QEvent::KeyPress:
	{
		if (!TestCase::CheckIsEvent())
		{
			const QKeyEvent* key_event{ dynamic_cast<QKeyEvent*>(event) };

			TestCase::Log("Key pressed " + key_event->text(), TestCase::kEvent);
			if (TestCase::is_record_)
			{
				TestCaseStep case_step{ TestCase::GetCaseStep(TestCase::current_url_) };

				switch (key_event->key())
				{
				case Qt::Key_Backspace:
				case Qt::Key_Delete:
				case Qt::Key_Left:
				case Qt::Key_Right:
				case Qt::Key_Up:
				case Qt::Key_Down:
				case Qt::Key_Home:
				case Qt::Key_End:
				case Qt::Key_PageUp:
				case Qt::Key_PageDown:
				case Qt::Key_Shift:
				case Qt::Key_Control:
				case Qt::Key_Alt:
				case Qt::Key_Meta:
				case Qt::Key_CapsLock:
				case Qt::Key_NumLock:
				case Qt::Key_ScrollLock:
				case Qt::Key_F1:
				case Qt::Key_F2:
				case Qt::Key_F3:
				case Qt::Key_F4:
				case Qt::Key_F5:
				case Qt::Key_F6:
				case Qt::Key_F7:
				case Qt::Key_F8:
				case Qt::Key_F9:
				case Qt::Key_F10:
				case Qt::Key_F11:
				case Qt::Key_F12:
					case_step.InsertNonAutomatic("@{KEY, " + QString::number(key_event->key()) + "}@");
					break;
				default:
					case_step.InsertNonAutomatic("@{SEND_TEXT, \"" + key_event->text() + "\"}@");
					break;
				}
				TestCase::InsertCaseStep(TestCase::current_url_, case_step);
				TestCase::SaveRecord();
			}
		}
		return false;
	}

	default:
		// standard event processing
		return QObject::eventFilter(obj, event);
	}
}
#endif