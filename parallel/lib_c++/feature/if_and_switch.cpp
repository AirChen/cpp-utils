在C++17标准中，if控制结构可以在执行条件语句之前先执行一个初始化语句。
bool foo()
{
  return true;
}
bool bar() {
    return false;
}
int main()
{
  if (bool b = foo(); b) {
       std::cout << std::boolalpha << "good! foo()=" << b << std::endl;
  } else if (bool t = bar(); !t) {
       std::cout << std::boolalpha << "good! bar()=" << t << std::endl;
  } else {
       std::cout << "bad!" << std::endl;
  }
}

注意：
1. b 的生命周期贯穿整个 if 语句，即使在 else / else if 分支中也可以访问。
2. t 的生命周期贯穿 else if 和 else 分支。

应用场景 加锁

std::mutex mx;
bool flag;
if (std::lock_guard lk(mx); flag) {
    // ...
}

其他：
switch 语句也支持初始化语句。
std::condition_variable cv;
std::mutex cv_m;
int main()
{
  switch (std::unique_lock<std::mutex> lk(cv_m); cv.wait_for(lk, 100ms))
  {
  case std::cv_status::timeout:
       break;
  case std::cv_status::no_timeout:
       break;
  }
}