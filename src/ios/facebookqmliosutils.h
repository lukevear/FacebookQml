#ifndef FACEBOOKQMLIOSUTILS
#define FACEBOOKQMLIOSUTILS

#include <QVariant>
#include <QMapIterator>
#import "Foundation/Foundation.h"

class FacebookQmlIosUtils
{

public:

    static NSMutableDictionary* mapToDictionary(QMap<QString, QVariant> map)
    {
        NSMutableDictionary *dictionary = [[NSMutableDictionary alloc] initWithCapacity:static_cast<NSUInteger>(map.count())];

        QMapIterator<QString, QVariant> i(map);
        while (i.hasNext()) {
            i.next();

            if (! i.value().isValid()) {
                continue;
            }

            if (i.value().type() == QVariant::String) {
                [dictionary setObject:i.value().toString().toNSString() forKey:i.key().toNSString()];
            } else if (i.value().canConvert(QMetaType::Double)) {
                [dictionary setObject:[[NSNumber alloc] initWithDouble:i.value().toDouble()] forKey:i.key().toNSString()];
            }
        }

        return dictionary;
    }

    static NSMutableArray* stringListToArray(QStringList list)
    {
        NSMutableArray *array = [[NSMutableArray alloc] init];
        for (int i = 0; i < list.size(); ++i) {
            [array addObject:list.at(i).toNSString()];
        }

        return array;
    }


};

#endif // FACEBOOKQMLIOSUTILS
