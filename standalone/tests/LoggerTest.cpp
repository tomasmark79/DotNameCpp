// MIT License Copyright (c) 2024-2025 Tomáš Mark

#include "../../src/Logger/Logger.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <thread>
#include <chrono>

class LoggerTest : public ::testing::Test {
protected:
  void SetUp () override {
    // Reset logger to default state before each test
    Logger& logger = Logger::getInstance ();
    logger.setLevel (Logger::Level::LOG_INFO);
    Logger::setAddNewLine (true);
    logger.noHeader (false);
    logger.setHeaderName ("DotNameLib");
    logger.disableFileLogging ();
  }

  void TearDown () override {
    // Clean up any test files
    std::remove ("test_log.txt");
  }
};

TEST_F (LoggerTest, BasicLogging) {
  Logger& logger = Logger::getInstance ();

  // These should not throw and should work
  EXPECT_NO_THROW (logger.debug ("Debug message", "BasicLogging"));
  EXPECT_NO_THROW (logger.info ("Info message", "BasicLogging"));
  EXPECT_NO_THROW (logger.warning ("Warning message", "BasicLogging"));
  EXPECT_NO_THROW (logger.error ("Error message", "BasicLogging"));
  EXPECT_NO_THROW (logger.critical ("Critical message", "BasicLogging"));
}

TEST_F (LoggerTest, LogLevelFiltering) {
  Logger& logger = Logger::getInstance ();

  // Test setting and getting log level
  logger.setLevel (Logger::Level::LOG_WARNING);
  EXPECT_EQ (logger.getLevel (), Logger::Level::LOG_WARNING);

  // Reset to default for other tests
  logger.setLevel (Logger::Level::LOG_INFO);
  EXPECT_EQ (logger.getLevel (), Logger::Level::LOG_INFO);
}

TEST_F (LoggerTest, StreamLogging) {
  // Test that stream logging compiles and runs without error
  EXPECT_NO_THROW ({
    LOG_I_STREAM << "Stream info: " << 42 << " value";
    LOG_W_STREAM << "Stream warning: " << "warning!";
    LOG_E_STREAM << "Stream error: " << "error with number " << 123;
  });
}

TEST_F (LoggerTest, FormattedLogging) {
  // Test formatted logging
  EXPECT_NO_THROW ({
    LOG_I_FMT ("Formatted message: number {}, text '{}'", 42, "hello");
    LOG_W_FMT ("Warning with {} parameters: {}, {}, {}", 3, "first", "second", "third");
    LOG_E_FMT ("Error in file '{}' at line {}", "test.cpp", 100);
  });
}

TEST_F (LoggerTest, MacroFunctionality) {
  // Test that all macros work without throwing
  EXPECT_NO_THROW ({
    LOG_I_MSG ("Test INFO macro");
    LOG_W_MSG ("Test WARNING macro");
    LOG_E_MSG ("Test ERROR macro");
    LOG_C_MSG ("Test CRITICAL macro");

    // Debug macro should work in debug mode, be no-op in release
    LOG_D_MSG ("Test DEBUG macro");
  });
}

TEST_F (LoggerTest, HeaderConfiguration) {
  Logger& logger = Logger::getInstance ();

  // Test header name change
  EXPECT_NO_THROW (logger.setHeaderName ("TESTAPP"));
  EXPECT_NO_THROW (logger.info ("Message with custom name", "HeaderConfiguration"));

  // Test header component control
  EXPECT_NO_THROW (logger.showHeaderTime (false));
  EXPECT_NO_THROW (logger.info ("Message without time", "HeaderConfiguration"));

  EXPECT_NO_THROW (logger.showHeaderCaller (false));
  EXPECT_NO_THROW (logger.info ("Message without caller", "HeaderConfiguration"));

  EXPECT_NO_THROW (logger.showHeaderLevel (false));
  EXPECT_NO_THROW (logger.info ("Message without level", "HeaderConfiguration"));

  // Test complete header removal
  EXPECT_NO_THROW (logger.noHeader (true));
  EXPECT_NO_THROW (logger.info ("Message without header", "HeaderConfiguration"));

  // Test restoration
  EXPECT_NO_THROW (logger.noHeader (false));
  EXPECT_NO_THROW (logger.setHeaderName ("DotNameLib"));
}

TEST_F (LoggerTest, NewLineControl) {
  Logger& logger = Logger::getInstance ();

  // Test newline control
  EXPECT_NO_THROW (Logger::setAddNewLine (true));
  EXPECT_TRUE (Logger::isAddNewLine ());
  EXPECT_NO_THROW (logger.info ("First message", "NewLineControl"));
  EXPECT_NO_THROW (logger.info ("Second message", "NewLineControl"));

  EXPECT_NO_THROW (Logger::setAddNewLine (false));
  EXPECT_FALSE (Logger::isAddNewLine ());
  EXPECT_NO_THROW (logger.info ("Third", "NewLineControl"));
  EXPECT_NO_THROW (logger.info (" fourth", "NewLineControl"));

  // Reset to default
  EXPECT_NO_THROW (Logger::setAddNewLine (true));
}

TEST_F (LoggerTest, FileLogging) {
  Logger& logger = Logger::getInstance ();

  // Test file logging enable/disable
  EXPECT_TRUE (logger.enableFileLogging ("test_log.txt"));

  EXPECT_NO_THROW (logger.info ("Test message to file", "FileLogging"));
  EXPECT_NO_THROW (logger.warning ("Test warning to file", "FileLogging"));
  EXPECT_NO_THROW (logger.error ("Test error to file", "FileLogging"));

  EXPECT_NO_THROW (logger.disableFileLogging ());

  // Verify file exists and has content
  std::ifstream file ("test_log.txt");
  EXPECT_TRUE (file.is_open ());
  if (file.is_open ()) {
    std::string line;
    int lineCount = 0;
    while (std::getline (file, line)) {
      lineCount++;
      EXPECT_FALSE (line.empty ());
    }
    EXPECT_EQ (lineCount, 3); // Should have 3 log entries
    file.close ();
  }
}

TEST_F (LoggerTest, ThreadSafety) {
  const int numThreads = 3;
  const int messagesPerThread = 5;
  std::vector<std::thread> threads;

  // Lambda function for thread logging
  auto logFunction = [] (int threadId) {
    for (int i = 0; i < messagesPerThread; ++i) {
      LOG_I_FMT ("Thread {} - message {}", threadId, i);
      std::this_thread::sleep_for (std::chrono::milliseconds (1));
    }
  };

  // Create and start threads
  threads.reserve (numThreads);
  for (int i = 0; i < numThreads; ++i) {
    threads.emplace_back (logFunction, i + 1);
  }

  // Wait for all threads to complete
  for (auto& t : threads) {
    EXPECT_NO_THROW (t.join ());
  }

  // If we get here without hanging or crashing, thread safety works
  SUCCEED ();
}

TEST_F (LoggerTest, LevelToString) {
  Logger& logger = Logger::getInstance ();

  // Test level to string conversion
  EXPECT_EQ (logger.levelToString (Logger::Level::LOG_DEBUG), "DBG");
  EXPECT_EQ (logger.levelToString (Logger::Level::LOG_INFO), "INF");
  EXPECT_EQ (logger.levelToString (Logger::Level::LOG_WARNING), "WRN");
  EXPECT_EQ (logger.levelToString (Logger::Level::LOG_ERROR), "ERR");
  EXPECT_EQ (logger.levelToString (Logger::Level::LOG_CRITICAL), "CRI");
}

TEST_F (LoggerTest, ColorMethods) {
  Logger& logger = Logger::getInstance ();

  // Test that color methods don't crash
  EXPECT_NO_THROW (logger.setConsoleColor (Logger::Level::LOG_INFO));
  EXPECT_NO_THROW (logger.setConsoleColor (Logger::Level::LOG_WARNING));
  EXPECT_NO_THROW (logger.setConsoleColor (Logger::Level::LOG_ERROR));
  EXPECT_NO_THROW (logger.resetConsoleColor ());
}

// Test that demonstrates the singleton behavior
TEST_F (LoggerTest, SingletonBehavior) {
  Logger& logger1 = Logger::getInstance ();
  Logger& logger2 = Logger::getInstance ();

  // Should be the same instance
  EXPECT_EQ (&logger1, &logger2);

  // Changes to one should affect the other
  logger1.setHeaderName ("SINGLETON_TEST");
  // Both should now have the same header name (we can't directly test this
  // without exposing the internal state, but the fact that they're the same
  // instance proves it)
  EXPECT_EQ (&logger1, &logger2);
}
