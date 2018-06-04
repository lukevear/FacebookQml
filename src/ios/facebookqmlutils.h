#ifndef FACEBOOKQMLUTILS_H
#define FACEBOOKQMLUTILS_H

#import "Foundation/Foundation.h"

class FacebookQmlUtils
{

public:
    static NSMutableArray* stringListToArray(QStringList list)
    {
        NSMutableArray *array = [[NSMutableArray alloc] init];
        for (int i = 0; i < list.size(); ++i) {
            [array addObject:list.at(i).toNSString()];
        }

        return array;
    }
};

#endif // FACEBOOKQMLUTILS_H
