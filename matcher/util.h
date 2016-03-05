//
//  util.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/4/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#ifndef util_h
#define util_h

template <class Container>
void STLDeleteValues(Container *container) {
    for (auto iter = container->begin(); iter != container->end(); ++iter) {
        delete (*iter);
    }
    container->clear();
}
#endif /* util_h */
