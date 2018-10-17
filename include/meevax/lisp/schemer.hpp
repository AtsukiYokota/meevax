#ifndef INCLUDED_MEEVAX_LISP_SCHEMER_HPP
#define INCLUDED_MEEVAX_LISP_SCHEMER_HPP

#include <iostream>
#include <regex>
#include <string>

#include <meevax/lisp/cell.hpp>
#include <meevax/lisp/reader.hpp>
#include <meevax/lisp/table.hpp>
#include <meevax/posix/noncanonical.hpp>

// 第一目標：ASTがインクリメンタルに構築できる

namespace meevax::lisp
{
  class schemer
  {
    cursor data;

  public:
    schemer()
      : data {symbols.intern("nil")}
    {}

    void operator_v1()
    {
      for (std::string buffer {}; true; ) try
      {
        // 描画（ゆくゆくはASTの書き出しのみに置き換えられるべき）
        std::cout << ">> " << (buffer += std::cin.get()) << std::endl;

        // ASTの構築（不正な場合は例外を投げて継続）
        const auto well_formed_expression {read(buffer)};

        std::cout << "\n=> " << eval(well_formed_expression) << "\n\n";
        buffer.clear();
      }
      catch (const std::string&) // unbalance expression
      {}
    }

    void operator_v2()
    {
      for (std::string buffer {}; true; ) try
      {
        const auto sequence {read_sequence()};

        if (sequence == "\\e")
        {
          const auto well_formed_expression {read(buffer)};
          std::cout << "=> " << eval(well_formed_expression) << std::endl;
          buffer.clear();
        }
        else
        {
          std::cout << "\r\e[K>> " << (buffer += sequence) << " == " << read(buffer) << std::endl;
        }
      }
      catch (const std::string&)
      {
        std::cout << "(ill-formed)";
      }
    }

    void operator()()
    {
      auto cursor {data};
      std::size_t distance {0};

      enum class semantics
      {
        normal, insert, command
      } semantic_cursor;

      while (true)
      {
        const auto s {read_sequence()};

        if (s == "\\e")
        {
          semantic_cursor = semantics::normal;
          std::cout << "semantic cursor -> normal" << std::endl;
          continue;
        }
        else if (s == "i" && semantic_cursor != semantics::insert)
        {
          semantic_cursor = semantics::insert;
          std::cout << "semantic cursor -> insert" << std::endl;
          continue;
        }
        else if (s == ";" && semantic_cursor != semantics::command)
        {
          semantic_cursor = semantics::command;
          std::cout << "semantic cursor -> command" << std::endl;
          continue;
        }

        switch (semantic_cursor)
        {
        case semantics::insert:
          if (s == "(")
          {
            std::cout << "open" << std::endl;

            std::cout << "consing empty list to cursor " << cursor << std::endl;
            cursor = symbols.intern("nil") | cursor;
          }
          break;

        case semantics::normal:
          break;

        case semantics::command:
          if (s == "q")
          {
            std::cout << "quit" << std::endl;
            std::exit(boost::exit_success);
          }
          break;
        }

        std::cout << data << std::endl;
      }
    }

  protected:
    // プリント可能な形でシーケンスを返すことに注意
    std::string read_sequence()
    {
      static const std::regex escape_sequence {"^\\\e\\[(\\d*;?)+(.|~)$"};

      std::string buffer {static_cast<char>(std::cin.get())};

      switch (buffer[0])
      {
      case '\e':
        if (!std::cin.rdbuf()->in_avail())
        {
          return {"\\e"};
        }
        else while (!std::regex_match(buffer, escape_sequence))
        {
          buffer.push_back(std::cin.get());
        }
        return std::string {"\\e["} + std::string {std::begin(buffer) + 2, std::end(buffer)};

      case '\n':
        return {"\\n"};

      default:
        return buffer;
      }
    }
  };
} // namespace meevax::lisp

#endif // INCLUDED_MEEVAX_LISP_SCHEMER_HPP
