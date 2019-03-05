#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>

int partition(void *d, int compare(void *d, int e1, int e2), void _swap(void *d, int e1, int e2), const int first, const int last)
{
  int pivot = first;
  int low = first;
  int i = first + 1;
  while (i <= last)
  {
#ifdef ESP32
#elif defined(ESP8266)

    // Keep the watcdog happy
    
    ESP.wdtFeed();
#endif
    
    if (compare(d, i,pivot) < 0)
    {
      low++;
      _swap(d, i, low);

    }
    i++;
  }

#ifdef ESP32
#elif defined(ESP8266)

    // Keep the watcdog happy
    
    ESP.wdtFeed();
#endif

  _swap(d, first, low);
  return low;
}

void SortAscending(void *d, int compare(void *d, int e1, int e2), void _swap(void *d, int e1, int e2), const int first, const int last) {

  if (first < last) {
    
    int pivot_pos = partition(d,compare,_swap, first, last);

#ifdef ESP32
#elif defined(ESP8266)

    // Keep the watcdog happy
    
    ESP.wdtFeed();
#endif

    SortAscending(d, compare, _swap, first, pivot_pos - 1);
    
#ifdef ESP32
#elif defined(ESP8266)

    // Keep the watcdog happy
    
    ESP.wdtFeed();
#endif
    SortAscending(d, compare, _swap, pivot_pos + 1, last);
  }
  
}
