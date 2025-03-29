#include "hierarchical_shared_mutex.h"

int main() {
    hierarchical_shared_mutex mutex_(100);

    std::unique_lock unlock(mutex_);
    std::shared_lock shlock(mutex_);

    return 0;
}