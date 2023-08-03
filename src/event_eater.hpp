#pragma once
#ifndef EVENT_EATER_HPP
#define EVENT_EATER_HPP

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QQuickWidget>

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
				TestCase::Log("Mouse pressed " + QString::number(mouse_event->pos().x()) + ", " + QString::number(mouse_event->pos().y()), TestCase::LogType::kEvent);
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

		case QEvent::FocusOut:
		{
			foreach(QObject * object, TestCase::web_view_->children())
			{
				if (QWidget * widget{ qobject_cast<QWidget*>(object) })
				{
					if (widget != TestCase::web_view_default_widget_)
					{
						//widget->removeEventFilter(this);
						widget->installEventFilter(this);
						TestCase::web_view_widget_ = widget;
					}
				}
			}
			break;
		}

		case QEvent::FocusIn:
		{
			if (TestCase::web_view_widget_ != TestCase::web_view_default_widget_)
			{
				bool flag{ true };
				foreach(QObject * object, TestCase::web_view_->children())
				{
					if (const QQuickWidget * widget{ qobject_cast<QQuickWidget*>(object) })
					{
						if (widget != TestCase::web_view_default_widget_)
						{
							flag = false;
						}
					}
				}
				if (flag)
				{
					TestCase::web_view_widget_ = TestCase::web_view_default_widget_;
				}
			}
			break;
		}

		case QEvent::KeyPress:
		{
			if (!TestCase::CheckIsEvent())
			{
				const QKeyEvent* key_event{ dynamic_cast<QKeyEvent*>(event) };

				/*if (key_event->matches(QKeySequence::Copy))
				{
					qDebug() << "Copy";
				}
				if (key_event->matches(QKeySequence::Paste))
				{
					qDebug() << "Paste";
				}
				if (key_event->matches(QKeySequence::Cut))
				{
					qDebug() << "Cut";
				}
				if (key_event->matches(QKeySequence::Undo))
				{
					qDebug() << "Undo";
				}
				if (key_event->matches(QKeySequence::Redo))
				{
					qDebug() << "Redo";
				}
				if (key_event->matches(QKeySequence::SelectAll))
				{
					qDebug() << "SelectAll";
				}*/

				TestCase::Log("Key pressed " + key_event->text(), TestCase::LogType::kEvent);
				if (TestCase::is_record_)
				{
					TestCaseStep case_step{ TestCase::GetCaseStep(TestCase::current_url_) };

					switch (key_event->key())
					{
						case Qt::Key_Shift:
						case Qt::Key_CapsLock:
							break;
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
						case Qt::Key_Control:
						case Qt::Key_Alt:
						case Qt::Key_Meta:
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
							QList<QString> non_automatic{ case_step.GetNonAutomatic() };
							const QString last_non_automatic{ non_automatic.takeLast() };
							if (const QRegularExpressionMatch match{ TestCase::regex_send_text_.match(last_non_automatic.trimmed()) }; match.hasMatch())
							{
								QList<QString> list_command{
									match.captured().trimmed().remove(0, 2).remove(QRegularExpression("}@$")).split(',')
								};

								// Remove SEN_TEXT
								list_command.removeFirst();

								// Take Text
								const QString text{
									list_command.join(',').trimmed().remove(QRegularExpression(R"(^")")).remove(
										QRegularExpression(R"("$)"))
								};

								const QString new_last_non_automatic{ text + key_event->text() };

								case_step.ReplaceLastNonAutomatic("@{SEND_TEXT, \"" + new_last_non_automatic + "\"}@");
							}
							else
							{
								case_step.InsertNonAutomatic("@{SEND_TEXT, \"" + key_event->text() + "\"}@");
							}
							break;
					}
					TestCase::InsertCaseStep(TestCase::current_url_, case_step);
					TestCase::SaveRecord();
				}
			}
			return false;
		}

		case QEvent::Wheel:
		{
			if (!TestCase::CheckIsEvent())
			{
				const QWheelEvent* wheel_event = dynamic_cast<QWheelEvent*>(event);
				QString wheel_position{ QString::number(wheel_event->position().x()) + "," + QString::number(wheel_event->position().y()) };
				wheel_event->angleDelta().y() > 0 ? TestCase::Log("scroll up " + wheel_position, TestCase::LogType::kEvent) : TestCase::Log("scroll down " + wheel_position, TestCase::LogType::kEvent);
				if (TestCase::is_record_)
				{
					TestCaseStep case_step{ TestCase::GetCaseStep(TestCase::current_url_) };
					case_step.InsertNonAutomatic("@{SCROLL, "
						+ QString::number(wheel_event->position().x()) + ", "
						+ QString::number(wheel_event->position().y()) + ", "
						+ QString::number(wheel_event->globalPosition().x()) + ", "
						+ QString::number(wheel_event->globalPosition().y()) + ", "
						+ QString::number(wheel_event->pixelDelta().x()) + ", "
						+ QString::number(wheel_event->pixelDelta().y()) + ", "
						+ QString::number(wheel_event->angleDelta().x()) + ", "
						+ QString::number(wheel_event->angleDelta().y()) + "}@");
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
	return false;
}
#endif
