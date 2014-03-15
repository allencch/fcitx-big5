fcitx-big5
==========

Convert GB18030 to BIG5 as UTF8.

The first intention of this project to start is to solve a problem in a game using WINE.
This is because the game can only run in LANG=zh_CN.UTF-8, yet the game itself is Traditional Chinese. This results that, when input a character, it is converted to BIG5 as the Traditional Chinese, which is incorrect. Such as entering "一", the result is "珨". Thus, in order to solve this problem, this module is to convert the UTF8 to GB18030 then BIG5 to UTF8, so that it makes the "珨" to "一" again.
