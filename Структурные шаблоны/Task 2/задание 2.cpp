#include <string>
#include <iostream>

class VeryHeavyDatabase {
public:
    virtual std::string GetData(const std::string& key) const {
        return "value";
    }

    virtual ~VeryHeavyDatabase() = default;
};

class OneShotDB : public VeryHeavyDatabase {
private:
    VeryHeavyDatabase* real_object_;
    mutable size_t remaining_shots_;

public:
    explicit OneShotDB(VeryHeavyDatabase* real_object, size_t shots = 1)
        : real_object_(real_object), remaining_shots_(shots) {}

    std::string GetData(const std::string& key) const override {
        if (remaining_shots_ > 0) {
            remaining_shots_--;
            return real_object_->GetData(key);
        }
        else {
            return "error";
        }
    }
};

int main() {
    auto real_db = VeryHeavyDatabase();
    auto limit_db = OneShotDB(std::addressof(real_db), 2);

    std::cout << limit_db.GetData("key") << std::endl;
    std::cout << limit_db.GetData("key") << std::endl;
    std::cout << limit_db.GetData("key") << std::endl;

    return 0;
}