class Counter {
  unsigned x = 0;

 public:
  unsigned get() const { return x; }
  Counter operator++(int) {
    Counter prev = *this;
    x++;
    return prev;
  }
};
