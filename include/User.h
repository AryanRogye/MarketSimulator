#ifndef USER_H
#define USER_H

#include <mutex>
class User {
    public:
        User(double cash);
        double getBalance();
    private:
        double cash;
        std::mutex mutex;
};

#endif // USER_H
