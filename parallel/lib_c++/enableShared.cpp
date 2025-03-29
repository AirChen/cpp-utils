#include <memory>
#include <iostream>

class ShareMe: public std::enable_shared_from_this<ShareMe> {
public:
    std::shared_ptr<ShareMe> getShared(){
        return shared_from_this();
    }
};

int main() {
    std::shared_ptr<ShareMe> shareMe(new ShareMe);
    std::shared_ptr<ShareMe> shareMe1 = shareMe->getShared();
    std::cout << (void*)shareMe.get() << '\n'; // 0x152d010
    std::cout << (void*)shareMe1.get() << '\n'; // 0x152d010
    std::cout << shareMe.use_count() << '\n'; // 2
    return 0;
}