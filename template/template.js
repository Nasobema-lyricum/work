//  ==============================================================
/**
    @file   template.js
    @brief  bgxxx for xxxxxx

            javascriptでクラスみたいなことするテンプレート
            

    @author kurosawa
    @date   2014/08/21

    Copyright (C) 2015 Roland Corporation.
 */
//  ==============================================================
var Foo = (function(){

    // Fooクラスのコンストラクタ
    var Foo = function(a){

        // public: なメンバ変数 bar（パブリックなメンバ変数）
        this.bar = a;

        // private: なメンバ変数 _baz（プライベートなメンバ変数）
        this._baz = "initial value";
    };

    // public: なメソッド qux
    Foo.prototype.qux = function(){
      console.log(this.bar);
    };

    // private: なメソッド _foobar
    Foo.prototype._foobar = function(){
        console.log("foo" + this.bar);
    };

    // public: static なメンバ変数 Hoge（パブリックなクラス変数）
    Foo.Hoge = "this is public";

    // public: static なメソッド Fuga（パブリックなクラスメソッド）
    Foo.Fuga = function(){
        console.log("this is public class method");
    };

    // private: static なメンバ変数 Moge（プライベートなクラス変数）
    var Moge = "this is private";

    // private: static なメソッド Piyo（プライベートなクラスメソッド）
    var Piyo = function(){
        console.log("this is private class method.");
    };

    return Foo;
})();

/* vim: set ts=8 sw=4 sts=4 expandtab: */
