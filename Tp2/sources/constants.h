#ifndef CONCUSQL_CONSTANTS_H
#define CONCUSQL_CONSTANTS_H

#include <string>

static const int NAME_SIZE = 60;
static const int ADDRESS_SIZE = 120;
static const int PHONE_SIZE = 13;

static const int SERVER_ID = 1;

static const std::string SERVER_TMP_FILE = "server.tmp";
static const std::string DATABASE_FILE = "database.db";

static const std::string SERVER_LOG_FILE = "log_server.log";
static const std::string CLIENT_LOG_FILE_HEAD = "log_client";
static const std::string CLIENT_LOG_FILE_EXTENSION = ".log";

static const int SERVER_DEBUG_TIME = 5;

static const int SERVER_ERROR = -1;
static const int REQUEST_ERROR = 0;
static const int REQUEST_SUCCESS = 1;

static const int CREATE_RECORD = 1;
static const int UPDATE_RECORD = 2;
static const int RETRIVE_RECORD = 3;

static const int MENU_CREATE = 1;
static const int MENU_UPDATE = 2;
static const int MENU_RETRIEVE = 3;
static const int MENU_EXIT = 4;

#endif //CONCUSQL_CONSTANTS_H
