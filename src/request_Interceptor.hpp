#pragma once
#ifndef REQUEST_INTERCEPTOR_HPP
#define REQUEST_INTERCEPTOR_HPP
#include <QWebEngineUrlRequestInterceptor>
#include <QDebug>

class RequestInterceptor final : public QWebEngineUrlRequestInterceptor
{
public:
	explicit RequestInterceptor(QObject* parent = nullptr) : QWebEngineUrlRequestInterceptor(parent) {}
	virtual void interceptRequest(QWebEngineUrlRequestInfo& info) override;
};

inline void RequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo& info)
{
	qDebug() << "Request URL:" << info.requestUrl();
}
#endif