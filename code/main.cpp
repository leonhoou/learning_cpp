#include <iostream>
#include <memory>
#include <vector>

/**
 * unique_ptr
 * 1.默认情况下unique_ptr的大小和原始指针一样；unique_ptr不允许拷贝，只能移动；
 * 2.unique_ptr作为继承层次结构中的对象的工厂函数返回类型：1.原始指针的生命周期程序员可能不好控制；2.使用unique_ptr自动生命周期；
 * 3.智能指针的自定义deleter，unique_ptr的大小会变化；
 */
/**
 * shared_ptr
 * 1.比原始指针大，内部存有一个指向数据的指针和一个指向ControlBlock（分配在堆上）的指针；所以shared_ptr的大小不会因为自定义deleter而改变；
 * 2.性能问题：1.内存大小是原始指针的2倍；2.引用计数的内存是动态分配的；3.引用计数的操作必须是原子的；
 * 3.ControlBlock生成时机：1.std::make_shared；2.通过unique_ptr构造shared_ptr；3.在shared_ptr的构造函数中传入一个裸指针；
 * 4.shared_ptr可能的问题：2个ControlBlock和一个obj关联到一起时会出现重复释放的风险；
 * 5.使用this作为std::shared_ptr构造函数实参
 * 6.shared_ptr不支持数组，shared_ptr<T[]>
 */
/**
 * weak_ptr
 * 1.不能单独使用，从shared_ptr上构建；
 * 2.有啥用？监视者；解决循环引用
 */
/**
 * std::make_unique和std::make_shared
 * 1.减少代码重复；
 * 2.使用make_unique和make_shared更安全：实参计算顺序可能造成内存泄露；
 */

class Sample;
class SampleV2;

std::vector<std::shared_ptr<Sample>> container;
std::vector<std::shared_ptr<SampleV2>> container_v2;

class Sample {
 public:
  Sample() { std::cout << "default constructor\n"; }
  ~Sample() { std::cout << "destructor\n"; }
  void process() { container.emplace_back(this); }
};

class SampleV2 : public std::enable_shared_from_this<SampleV2> {
 public:
  SampleV2() { std::cout << "default constructor\n"; }
  ~SampleV2() { std::cout << "destructor\n"; }
  void process() { container_v2.emplace_back(shared_from_this()); }
};

auto MyDeleter = [](Sample *s) {
  std::cout << "MyDeleter called\n";
  delete s;
};

int main() {
  {
      // std::unique_ptr<Sample, decltype(MyDeleter)> s1{new Sample(),
      // MyDeleter}; std::shared_ptr<Sample> s2{new Sample(), MyDeleter};
  }

  {
    // 存在问题：重复释放
    Sample *s = new Sample();
    std::shared_ptr<Sample> ss1{s};
    std::shared_ptr<Sample> ss2{s};
  }
  std::cout << "-------- Program End --------" << std::endl;
}
