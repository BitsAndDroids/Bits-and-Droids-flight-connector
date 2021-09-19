#include "tests.h"

#include <settings/settingshandler.h>

#include <QtTest/QtTest>

Tests::Tests() {
  toUpper();
  checkSomething();
};
Tests::~Tests(){};
//! [0]

//! [1]
void Tests::initTestCase() { qDebug("Called before everything else."); }
void Tests::toUpper() {
  QString str = "Hello";
  QCOMPARE(str.toUpper(), QString("HELLO"));
}

void Tests::checkSomething() {
  SettingsHandler handler;
  QVERIFY2(handler.retrieveSetting("setKeys", "lastId")->toInt() < 0,
           "correct");
}
