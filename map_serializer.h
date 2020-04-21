//
// Created by elia on 21/04/2020.
//

#ifndef MAP_SERIALIZER_H
#define MAP_SERIALIZER_H

#include "mtm_map/map.h"

/**
 * serializerMapToString: Serialize Map data structure to string
 * @param map to serialize
 * @return char pointer to the beginning of the serialized map string
 */
char* serializerMapToString(Map map);

/**
 * serializerStringToMap: Restore serialized map string into Map structure
 * @param string_map serialized map string
 * @return Map structure made out of string_map
 */
Map serializerStringToMap(char* string_map);

#endif //MAP_SERIALIZER_H
