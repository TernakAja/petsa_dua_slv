#include "utils.h"

int getAverage(int data[], int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += data[i];
  }
  return size > 0 ? sum / size : 0;
}

void remoteSourceSynchronize() {
  // TODO:
  // 1. Load WiFi connection
  // 2. Check if deviceId exists in the database
  // 3. Send payload to remote database
}
