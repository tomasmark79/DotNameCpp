#include "../../src/Utils/Logger/ConsoleLogger.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <memory>

using namespace dotnamecpp::logging;

class ConsoleLoggerTest : public ::testing::Test {
protected:
  std::shared_ptr<ConsoleLogger> logger;

  void SetUp () override {
    logger = std::make_shared<ConsoleLogger> ();
    logger->setLevel (Level::LOG_INFO);
    logger->setHeaderName ("TestLogger");
    logger->visibleHeaders (true, true, true, true);
    logger->disableFileLogging ();
  }

  void TearDown () override {
    // Clean up any test files
    std::remove ("test_log.txt");
  }
};

TEST_F (ConsoleLoggerTest, BasicLogging) {
  // Test basic logging methods
  EXPECT_NO_THROW (logger->debug ("Debug message", "BasicLogging"));
  EXPECT_NO_THROW (logger->info ("Info message", "BasicLogging"));
  EXPECT_NO_THROW (logger->warning ("Warning message", "BasicLogging"));
  EXPECT_NO_THROW (logger->error ("Error message", "BasicLogging"));
  EXPECT_NO_THROW (logger->critical ("Critical message", "BasicLogging"));
}

TEST_F (ConsoleLoggerTest, LoggingWithoutCaller) {
  // Test logging without caller parameter
  EXPECT_NO_THROW (logger->debug ("Debug message"));
  EXPECT_NO_THROW (logger->info ("Info message"));
  EXPECT_NO_THROW (logger->warning ("Warning message"));
  EXPECT_NO_THROW (logger->error ("Error message"));
  EXPECT_NO_THROW (logger->critical ("Critical message"));
}

TEST_F (ConsoleLoggerTest, LogLevelFiltering) {
  // Test setting and getting log level
  logger->setLevel (Level::LOG_WARNING);
  EXPECT_EQ (logger->getLevel (), Level::LOG_WARNING);

  logger->setLevel (Level::LOG_DEBUG);
  EXPECT_EQ (logger->getLevel (), Level::LOG_DEBUG);

  logger->setLevel (Level::LOG_INFO);
  EXPECT_EQ (logger->getLevel (), Level::LOG_INFO);
}

TEST_F (ConsoleLoggerTest, StreamLogging) {
  // Test stream logging
  EXPECT_NO_THROW ({
    logger->stream (Level::LOG_INFO, "StreamTest") << "Stream info: " << 42 << " value";
    logger->stream (Level::LOG_WARNING) << "Stream warning without caller";
    logger->stream (Level::LOG_ERROR, "StreamTest")
      << "Stream error: " << "error with number " << 123;
  });
}

TEST_F (ConsoleLoggerTest, ConvenienceStreamMethods) {
  // Test convenience stream methods
  EXPECT_NO_THROW ({
    logger->debugStream () << "Debug stream message";
    logger->infoStream () << "Info stream: " << 42;
    logger->warningStream ("StreamTest") << "Warning stream with caller";
    logger->errorStream () << "Error stream: " << "error";
    logger->criticalStream () << "Critical stream";
  });
}

TEST_F (ConsoleLoggerTest, FormattedLogging) {
  // Test formatted logging with fmt
  EXPECT_NO_THROW ({
    logger->infoFmt ("Formatted message: number {}, text '{}'", 42, "hello");
    logger->warningFmt ("Warning with {} parameters: {}, {}, {}", 3, "first", "second", "third");
    logger->errorFmt ("Error in file '{}' at line {}", "test.cpp", 100);
    logger->debugFmt ("Debug value: {}", 999);
    logger->criticalFmt ("Critical error code: {}", 500);
  });
}

TEST_F (ConsoleLoggerTest, HeaderConfiguration) {
  // Test header name change
  EXPECT_NO_THROW (logger->setHeaderName ("TESTAPP"));
  EXPECT_NO_THROW (logger->info ("Message with custom name", "HeaderConfig"));

  // Test header component control
  EXPECT_NO_THROW (logger->showHeaderTime (false));
  EXPECT_NO_THROW (logger->info ("Message without time"));

  EXPECT_NO_THROW (logger->showHeaderCaller (false));
  EXPECT_NO_THROW (logger->info ("Message without caller", "HeaderConfig"));

  EXPECT_NO_THROW (logger->showHeaderLevel (false));
  EXPECT_NO_THROW (logger->info ("Message without level"));

  // Test complete header removal
  EXPECT_NO_THROW (logger->noHeader (true));
  EXPECT_NO_THROW (logger->info ("Message without header"));

  // Test restoration
  EXPECT_NO_THROW (logger->noHeader (false));
  EXPECT_NO_THROW (logger->setHeaderName ("TestLogger"));
}

TEST_F (ConsoleLoggerTest, VisibleHeaders) {
  // Test visibleHeaders method
  EXPECT_NO_THROW (logger->visibleHeaders (true, false, true, false));
  EXPECT_NO_THROW (logger->info ("Only name and caller visible", "VisibleTest"));

  EXPECT_NO_THROW (logger->visibleHeaders (false, true, false, true));
  EXPECT_NO_THROW (logger->info ("Only time and level visible"));

  // Reset to default
  EXPECT_NO_THROW (logger->visibleHeaders (true, true, true, true));
}

TEST_F (ConsoleLoggerTest, FileLogging) {
  // Test file logging enable/disable
  EXPECT_TRUE (logger->enableFileLogging ("test_log.txt"));

  EXPECT_NO_THROW (logger->info ("Test message to file", "FileLogging"));
  EXPECT_NO_THROW (logger->warning ("Test warning to file", "FileLogging"));
  EXPECT_NO_THROW (logger->error ("Test error to file", "FileLogging"));

  EXPECT_NO_THROW (logger->disableFileLogging ());

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

TEST_F (ConsoleLoggerTest, ThreadSafety) {
  const int numThreads = 3;
  const int messagesPerThread = 5;
  std::vector<std::thread> threads;

  // Lambda function for thread logging
  auto logFunction = [this] (int threadId) {
    for (int i = 0; i < messagesPerThread; ++i) {
      logger->infoFmt ("Thread {} - message {}", threadId, i);
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

TEST_F (ConsoleLoggerTest, LevelToString) {
  // Test level to string conversion
  EXPECT_EQ (ConsoleLogger::levelToString (Level::LOG_DEBUG), "DBG");
  EXPECT_EQ (ConsoleLogger::levelToString (Level::LOG_INFO), "INF");
  EXPECT_EQ (ConsoleLogger::levelToString (Level::LOG_WARNING), "WRN");
  EXPECT_EQ (ConsoleLogger::levelToString (Level::LOG_ERROR), "ERR");
  EXPECT_EQ (ConsoleLogger::levelToString (Level::LOG_CRITICAL), "CRI");
}

TEST_F (ConsoleLoggerTest, ColorMethods) {
  // Test that color methods don't crash
  EXPECT_NO_THROW (ConsoleLogger::setConsoleColor (Level::LOG_INFO));
  EXPECT_NO_THROW (ConsoleLogger::setConsoleColor (Level::LOG_WARNING));
  EXPECT_NO_THROW (ConsoleLogger::setConsoleColor (Level::LOG_ERROR));
  EXPECT_NO_THROW (ConsoleLogger::resetConsoleColor ());
}

TEST_F (ConsoleLoggerTest, PolymorphicUsage) {
  // Test using logger through ILogger interface
  std::shared_ptr<ILogger> interfaceLogger = std::make_shared<ConsoleLogger> ();

  EXPECT_NO_THROW (interfaceLogger->debug ("Debug via interface"));
  EXPECT_NO_THROW (interfaceLogger->info ("Info via interface"));
  EXPECT_NO_THROW (interfaceLogger->warning ("Warning via interface"));
  EXPECT_NO_THROW (interfaceLogger->error ("Error via interface"));
  EXPECT_NO_THROW (interfaceLogger->critical ("Critical via interface"));

  // Test stream methods through interface
  EXPECT_NO_THROW (interfaceLogger->infoStream () << "Stream via interface: " << 42);
  EXPECT_NO_THROW (interfaceLogger->debugStream () << "Debug stream");

  // Test fmt methods through interface
  EXPECT_NO_THROW (interfaceLogger->infoFmt ("Formatted via interface: {}", 123));
  EXPECT_NO_THROW (interfaceLogger->errorFmt ("Error code: {}", 404));
}

TEST_F (ConsoleLoggerTest, MultipleInstances) {
  // Test that multiple logger instances work independently
  auto logger1 = std::make_shared<ConsoleLogger> ();
  auto logger2 = std::make_shared<ConsoleLogger> ();

  logger1->setHeaderName ("Logger1");
  logger2->setHeaderName ("Logger2");

  EXPECT_NO_THROW (logger1->info ("Message from logger1"));
  EXPECT_NO_THROW (logger2->info ("Message from logger2"));

  // They should be different instances
  EXPECT_NE (logger1.get (), logger2.get ());
}

TEST_F (ConsoleLoggerTest, MoveSemantics) {
  // Test move constructor
  ConsoleLogger logger1;
  logger1.setHeaderName ("MovedLogger");

  ConsoleLogger logger2 (std::move (logger1));
  EXPECT_NO_THROW (logger2.info ("Message from moved logger"));

  // Test move assignment
  ConsoleLogger logger3;
  logger3 = std::move (logger2);
  EXPECT_NO_THROW (logger3.info ("Message after move assignment"));
}

TEST_F (ConsoleLoggerTest, StreamWithMultipleValues) {
  // Test stream with various types
  EXPECT_NO_THROW ({
    logger->infoStream () << "Integer: " << 42 << ", Float: " << 3.14 << ", String: " << "test"
                          << ", Bool: " << true;
  });
}

TEST_F (ConsoleLoggerTest, EmptyMessages) {
  // Test logging empty messages
  EXPECT_NO_THROW (logger->info (""));
  EXPECT_NO_THROW (logger->debug ("", "EmptyTest"));
  EXPECT_NO_THROW (logger->infoStream () << "");
}

TEST_F (ConsoleLoggerTest, LongMessages) {
  // Test logging very long messages
  std::string longMessage (1000, 'X');
  EXPECT_NO_THROW (logger->info (longMessage));
  EXPECT_NO_THROW (logger->infoStream () << longMessage);
  EXPECT_NO_THROW (logger->infoFmt ("Long: {}", longMessage));
}
