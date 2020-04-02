#pragma once


template <typename Type>
class Singleton {
private:
    friend Type* Type::getInstance();

    // Return a pointer to the one true instance of the class.
    static Type* get()
    {
        if (!instance_) {
            instance_ = new Type;
        }
        return instance_;
    }

    static Type* get(QObject* parent)
    {
        if (!instance_) {
            instance_ = new Type(parent);
        }
        return instance_;
    }

    static void onExit(void* /*unused*/)
    {
        if (instance_) {
            delete instance_;
            instance_ = nullptr;
        }
    }

    static Type* instance_;
};

template<typename Type>
Type* Singleton<Type>::instance_ = nullptr;
