cmake_minimum_required(VERSION 3.16)

project(project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(project
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        UserManager.h
        UserManager.cpp
        DatabaseService.h
        DatabaseService.cpp
        TipManager.h
        TipManager.cpp
        BalanceCalculator.h
        BalanceCalculator.cpp
        PerformanceService.h
        PerformanceService.cpp
        UserRepository.h
        UserRepository.cpp
        logindialog.h logindialog.cpp logindialog.ui
        metricsdialog.h metricsdialog.cpp metricsdialog.ui
        addtipdialog.h addtipdialog.cpp addtipdialog.ui
        editlasttipdialog.h editlasttipdialog.cpp editlasttipdialog.ui
        balancedialog.h balancedialog.cpp balancedialog.ui
        usermanagementdialog.h usermanagementdialog.cpp usermanagementdialog.ui
        BackupService.h
        BackupService.cpp
        ReportGenerator.h
        ReportGenerator.cpp
        SqlTipMetricsRepository.h
        SqlTipMetricsRepository.cpp
        ITipMetricsRepository.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET project APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(project SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(project
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(project PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.project)
endif()
set_target_properties(project PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(project)
endif()

enable_testing()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

# Add the test executable for UserManager
add_executable(UserManagerTest
    tests/UserManagerTest.cpp
    UserManager.cpp
)

target_include_directories(UserManagerTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}          
)

target_link_libraries(UserManagerTest
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql 
)

add_test(NAME UserManagerTest COMMAND UserManagerTest)

# Add the test executable for TipManager
add_executable(TipManagerTest
    tests/TipManagerTest.cpp
    TipManager.cpp
)

target_include_directories(TipManagerTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TipManagerTest
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME TipManagerTest COMMAND TipManagerTest)

# Add the test executable for BackupServiceTest
add_executable(BackupServiceTest
    tests/BackupServiceTest.cpp
    BackupService.cpp
)

target_include_directories(BackupServiceTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}     
)

target_link_libraries(BackupServiceTest
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME BackupServiceTest COMMAND BackupServiceTest)

# Add the test executable for SqlTipMetricsRepositoryTest
add_executable(SqlTipMetricsRepositoryTest
    tests/SqlTipMetricsRepositoryTest.cpp
    SqlTipMetricsRepository.cpp
)

target_include_directories(SqlTipMetricsRepositoryTest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SqlTipMetricsRepositoryTest
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME SqlTipMetricsRepositoryTest COMMAND SqlTipMetricsRepositoryTest)

# Add the test executable for DatabaseServiceTest
add_executable(DatabaseServiceTest
    tests/DatabaseServiceTest.cpp
    DatabaseService.cpp
)

target_include_directories(DatabaseServiceTest PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(DatabaseServiceTest
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME DatabaseServiceTest COMMAND DatabaseServiceTest)

# Add the test executable for BalanceCalculatorTest
add_executable(BalanceCalculatorTest
    tests/BalanceCalculatorTest.cpp
    BalanceCalculator.cpp
)

target_include_directories(BalanceCalculatorTest PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BalanceCalculatorTest
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME BalanceCalculatorTest COMMAND BalanceCalculatorTest)

# Add the test executable for PerformanceServiceTest
add_executable(PerformanceServiceTest
    tests/PerformanceServiceTest.cpp
    PerformanceService.cpp
)

target_include_directories(PerformanceServiceTest PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(PerformanceServiceTest
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME PerformanceServiceTest COMMAND PerformanceServiceTest)

# Add the test executable for ReportGeneratorTest
add_executable(ReportGeneratorTest
    tests/ReportGeneratorTest.cpp
    ReportGenerator.cpp
)

target_include_directories(ReportGeneratorTest PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ReportGeneratorTest
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME ReportGeneratorTest COMMAND ReportGeneratorTest)

# Add the test executable for ReportGeneratorTest
add_executable(UserRepositoryTest
    tests/UserRepositoryTest.cpp
    UserRepository.cpp
)

target_include_directories(UserRepositoryTest PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(UserRepositoryTest
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME UserRepositoryTest COMMAND UserRepositoryTest)