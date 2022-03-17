# Banking-System

*Keep Track of Bank Customers Through Use of MySQL and Visual Studio*

## Requirements
- Visual Studio 2019 (with Desktop development with C++)
- MySQL C++ Driver (Connector/C++)
  - Version 8.0.28
  - Also ensure you download the zip file version with it extracted
- MySQL for Visual Studio
  - Version 1.2.10
- MySQL Router/Server
  - Version 8.0

## Configuration Notes
1. Right Click on Solution -> Properties
2. Ensure the configuration is set to **Release** and platform is set to **x64**
3. In Debugging (3rd item under Configuration Properties):
  - Ensure that the command arguments are set to the following:
    - `URL Username Password`
4. In C/C++ section
  - In General -> Additional Include Directories, add the **jdbc** folder from extracted zip file
  - In Preprocessor -> Preprocessor Definition, define **STATIC_CONCPP** as one of the macros
  - In Code Generation -> Runtime Library, select **Multi-threaded DLL (/MD)** 
5. In Linker section
  - In General -> Additional Library Directories, add the **vs14** folder from extracted zip file
  - In Input -> Additional Dependencies, add **mystaticcppcon-static.lib** from the **vs14** folder

### Additional Resources:
  - [Set Up Tutorial] https://www.youtube.com/watch?v=a_W4zt5sR1M&list=LL&index=2&ab_channel=BoostMyTool
  - [Using Connector/C++] https://dev.mysql.com/doc/dev/connector-cpp/8.0/usage.html
  - [Building Connector/C++] https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-apps-windows-visual-studio.html
  - Note: Using Legacy Code with Connector/C++ 1.1 (JDBC)

## Goal
- Use classes to gather information about customers
- Use regex to validate deposit and withdrawal balances
- Use Microsoft Visual Studio to build and run solutions
- Learn how to use MySQL Workbench and write MySQL Syntax
- Consult the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

## Possible Future Additions
- Implement a login system
  - Create this in HTML/CSS/Javascript instead of using C++
- Use multiple tables in MySQL to make connections with customer information
- Create a way to transfer money between two bank accounts
- Timestamps when information is modified in the database
- Time system for to keep track of loans or interests of a customer