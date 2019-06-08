#include <el/casting.h>
// http://llvm.org/docs/HowToSetUpLLVMStyleRTTI.html
// http://llvm.org/docs/ProgrammersManual.html#the-isa-cast-and-dyn-cast-templates
enum Estruct
{
    kB, kC
};

struct a {
    virtual ~a() {
        printf("Hello world");
    }
    virtual int id() const { return 0; }
};

struct b : public a{
    virtual ~b() {
        printf("Hello world2");
    }
    virtual int id() const { return 2; }

    static bool classof(const a* i) {
        return i->id() % 2 == 0;
    }
};

struct c : public b
{
    virtual ~c() {
        printf("Hello world3");
    }
    virtual int id() const { return 4; }
};

struct d : public a
{
    virtual int id() const { return 1; }

    static bool classof(const a* i) {
        return i->id() % 2;
    }
};


int main()
{
    auto i = std::make_unique<c>();
    std::unique_ptr<b> k = llvm::unique_dyn_cast<b>(i);
}

