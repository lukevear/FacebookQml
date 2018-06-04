#ifndef FACEBOOKQMLUTILS_H
#define FACEBOOKQMLUTILS_H

#include <QAndroidJniObject>
#include <QStringList>

class FacebookQmlUtils
{

public:
	static QAndroidJniObject stringListToArrayList(QStringList list)
	{
		QAndroidJniObject array = QAndroidJniObject("java/util/ArrayList");

	    foreach (const QString &str, list) {
			array.callMethod<jboolean>(
	            "add",
	            "(Ljava/lang/Object;)Z",
                QAndroidJniObject::fromString(str).object<jstring>()
        	);
	    }

		return array;
	}

};

#endif // FACEBOOKQMLUTILS_H
