#ifndef ECORE_API_H
#define ECORE_API_H

#define countof(array) (sizeof(array) / sizeof(array[0]))
#define bitof(type) (countof(type) * 8)

#define forever for (;;)

#define foreach(item, array)                                                   \
  for (int keep = 1, count = 0, size = sizeof(array) / sizeof *(array);        \
       keep && count != size; keep = !keep, count++)                           \
    for (item = (array) + count; keep; keep = !keep)

#endif // ECORE_API_H
