#ifndef TESTS_H
#define TESTS_H

#include <QObject>

class Tests : public QObject {
 public:
  Tests();
  ~Tests();

 private slots:
  void checkSomething();

  void initTestCase();

 private:
  void toUpper();
};

#endif  // TESTS_H
