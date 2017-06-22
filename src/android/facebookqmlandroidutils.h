#ifndef FACEBOOKQMLANDROIDUTILS
#define FACEBOOKQMLANDROIDUTILS

#include <QAndroidJniObject>
#include <QVariant>
#include <QMapIterator>
#include <QStringList>
#include <QStringListIterator>

class FacebookQmlAndroidUtils
{

public:

	static QAndroidJniObject getInstance()
	{
	    return QAndroidJniObject::callStaticObjectMethod("com/lukevear/facebookqml/FacebookQml", "getInstance", "()Lcom/lukevear/facebookqml/FacebookQml;");
	}

	static QAndroidJniObject getLogger()
	{
	    QAndroidJniObject _instance = getInstance();
	    if (! _instance.isValid()) {
	        return QAndroidJniObject();
	    }

	    return _instance.callObjectMethod("getLogger", "()Lcom/facebook/appevents/AppEventsLogger;");
	}

	static QAndroidJniObject mapToBundle(QMap<QString, QVariant> map)
	{
	    QAndroidJniObject bundle = QAndroidJniObject("android/os/Bundle");
	    if (! bundle.isValid()) {
	        return QAndroidJniObject();
	    }

	    QMapIterator<QString, QVariant> i(map);
	    while (i.hasNext()) {
	        i.next();

	        if (! i.value().isValid()) {
	            continue;
	        }

	        if (i.value().type() == QVariant::String) {
	            bundle.callMethod<void>(
	                "putString",
	                "(Ljava/lang/String;Ljava/lang/String;)V",
	                QAndroidJniObject::fromString(i.key()).object<jstring>(),
	                QAndroidJniObject::fromString(i.value().toString()).object<jstring>()
	            );
	        } else if (i.value().canConvert(QMetaType::Double)) {
	            bundle.callMethod<void>(
	                "putDouble",
	                "(Ljava/lang/String;D)V",
	                QAndroidJniObject::fromString(i.key()).object<jstring>(),
	                static_cast<jdouble>(i.value().toDouble())
	            );
	        }
	    }

	    return bundle;
	}

	static QAndroidJniObject stringListToArrayList(QStringList list)
	{
		QAndroidJniObject _array = QAndroidJniObject("java/util/ArrayList");

		QStringListIterator i(list);
		while (i.hasNext()) {
			_array.callMethod<jboolean>(
	            "add",
	            "(Ljava/lang/Object;)Z",
                QAndroidJniObject::fromString(i.next()).object<jstring>()
        	);
	    }

		return _array;
	}

};

#endif // FACEBOOKQMLANDROIDUTILS
