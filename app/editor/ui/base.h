#pragma once

#include "../include.h"
#include "imgui.h"

class UIState;
class UIClass;
class UIMgr;

enum class DirectEnum {
    kU, kD, kL, kR, LENGTH
};

// ---
//  �ؼ��б�
// ---
enum class UITypeEnum {
    kLAYOUT,                //  ����
    kTREEBOX,               //  ���ο�
    kTEXTBOX,               //  �ı���
    kIMAGEBOX,              //  ͼƬ��
    kCOMBOBOX,              //  ������
    kUICONVAS,              //  UI����
    kGLCONVAS,              //  GL����
};

// ---
//  �ؼ�����
// ---
enum class UIAlignEnum {
    kDEFAULT,               //  ��������
    kCLING_T = 1 << 1,      //  ����
    kCLING_B = 1 << 2,      //  ����
    kCLING_L = 1 << 3,      //  ����
    kCLING_R = 1 << 4,      //  ����
    kCENTER_H = 1 << 5,     //  ˮƽ����
    kCENTER_V = 1 << 6,     //  ��ֱ����
    kSTRETCH_H = 1 << 7,    //  ˮƽ����
    kSTRETCH_V = 1 << 8,    //  ��ֱ����
    LENGTH,
};

// ---
//  �ؼ��¼�
// ---
enum class UIEventEnum {
    kKEY,               //  ����
    kMOUSE_DOWN,        //  ���-����
    kMOUSE_RELEASE,     //  ���-̧��
    kMOUSE_HOVERED,     //  ���-����
    kMOUSE_CLICK,       //  ���-����
    kMOUSE_DCLICK,      //  ���-˫��
    kEDIT_TEXT_FINISH,  //  �������
};

// ---
//  �¼�����ֵ
// ---
enum class UIEventResultEnum {
    kSTOP,      //  ֹͣ����
    kPASS,      //  ����ͨ��
};

// ---
//  Ĭ��ֵ����
// ---
constexpr auto LAYOUT_STRETCH_BORDER = 5.0f;      //  Layout ���϶��߾�

// ---
//  �ַ���ת��������
// ---
inline bool     s2b(const std::string & s) { return s == "ok"; }
inline int      s2i(const std::string & s) { return std::stoi(s); }
inline float    s2f(const std::string & s) { return std::stof(s); }

inline glm::vec2 s2v2(const std::string & s)
{
    auto arr = string_tool::Split(s, " ");
    ASSERT_LOG(arr.size() == 2, "Arr Length: {0}!", arr.size());
    return glm::vec2(s2f(arr.at(0)), s2f(arr.at(1)));
}

inline glm::vec3 s2v3(const std::string & s)
{
    auto arr = string_tool::Split(s, " ");
    ASSERT_LOG(arr.size() == 3, "Arr Length: {0}!", arr.size());
    return glm::vec3(s2f(arr.at(0)), s2f(arr.at(1)), s2f(arr.at(2)));
}

inline glm::vec4 s2v4(const std::string & s)
{
    auto arr = string_tool::Split(s, " ");
    ASSERT_LOG(arr.size() == 4, "Arr Length: {0}!", arr.size());
    return glm::vec4(s2f(arr.at(0)), s2f(arr.at(1)), s2f(arr.at(2)), s2f(arr.at(3)));
}

inline std::vector<int> s2vi(const std::string & s)
{
    std::vector<int> ret;
    auto arr = string_tool::Split(s, " ");
    std::transform(arr.begin(), arr.end(), std::back_inserter(ret), 
        [] (const std::string & s) { return std::stoi(s); });
    return ret;
}

// ---
//  �Զ������� Get/Set
// ---
using CustomData = std::map<std::string, std::any>;

//  ��������
template <class T>
void __ParseUIData(CustomData & data, const std::string & key, const std::string & val)
{
    if constexpr (std::is_same_v<T, int>)           { data.emplace(key, s2i(val)); }
    else if constexpr (std::is_same_v<T, bool>)     { data.emplace(key, s2b(val)); }
    else if constexpr (std::is_same_v<T, float>)    { data.emplace(key, s2f(val)); }
    else if constexpr (std::is_same_v<T, std::string>)  { data.emplace(key, (val)); }
    else if constexpr (std::is_same_v<T, glm::vec2>)    { data.emplace(key, s2v2(val)); }
    else if constexpr (std::is_same_v<T, glm::vec3>)    { data.emplace(key, s2v3(val)); }
    else if constexpr (std::is_same_v<T, glm::vec4>)    { data.emplace(key, s2v4(val)); }
    else if constexpr (std::is_same_v<T, std::vector<int>>) { data.emplace(key, s2vi(val)); }
    else { static_assert(false); }
}

//  ��������
#define __REG_PARSE_UI_DATA(data, key, val, T, K)     if (#K == key) __ParseUIData<T>(data, key, val)

//  ��ȡ����
#define __REG_GET_UI_DATA(T, K, def)                                       \
    inline const T & __GetData##K(const CustomData & data)                 \
    {                                                                      \
        static T tmp = def;                                                \
        auto it = data.find(#K);                                           \
        if (it == data.end()) return tmp;                                  \
        return std::any_cast<const T &>(it->second);                       \
    }

//  ��������
#define __REG_SET_UI_DATA(T, K)                                            \
    inline void __SetData##K(CustomData & data, const T & val)             \
    {                                                                      \
        data[#K] = val;                                                    \
    }

__REG_GET_UI_DATA(int, Align, 0) //  ����
__REG_SET_UI_DATA(int, Align)

__REG_GET_UI_DATA(float, BorderNumber, 1) //  �߿����
__REG_SET_UI_DATA(float, BorderNumber)

__REG_GET_UI_DATA(bool, IsMulti, false) //  ����
__REG_SET_UI_DATA(bool, IsMulti)
__REG_GET_UI_DATA(bool, IsWindow, false) //  ����
__REG_SET_UI_DATA(bool, IsWindow)
__REG_GET_UI_DATA(bool, IsButton, false) //  ��ť
__REG_SET_UI_DATA(bool, IsButton)
__REG_GET_UI_DATA(bool, IsVisible, true) //  �ɼ�
__REG_SET_UI_DATA(bool, IsVisible)
__REG_GET_UI_DATA(bool, IsShowNav, false) //  �е�����
__REG_SET_UI_DATA(bool, IsShowNav)
__REG_GET_UI_DATA(bool, IsTextBox, false) //  �ı���
__REG_SET_UI_DATA(bool, IsTextBox)
__REG_GET_UI_DATA(bool, IsEditBox, false) //  �༭��
__REG_SET_UI_DATA(bool, IsEditBox)
__REG_GET_UI_DATA(bool, IsCanMove, false) //  �����ƶ�
__REG_SET_UI_DATA(bool, IsCanMove)
__REG_GET_UI_DATA(bool, EnabledKey, false) //  ���ü���
__REG_SET_UI_DATA(bool, EnabledKey)
__REG_GET_UI_DATA(bool, EnabledMouse, false) //  �������
__REG_SET_UI_DATA(bool, EnabledMouse)
__REG_GET_UI_DATA(bool, IsCanStretch, false) //  ��������
__REG_SET_UI_DATA(bool, IsCanStretch)
__REG_GET_UI_DATA(bool, IsFullScreen, false) //  ����ȫ��
__REG_SET_UI_DATA(bool, IsFullScreen)
__REG_GET_UI_DATA(bool, IsShowBorder, true) //  ��ʾ�߿�
__REG_SET_UI_DATA(bool, IsShowBorder)
__REG_GET_UI_DATA(bool, IsShowTitleBar, false) //  �б�����
__REG_SET_UI_DATA(bool, IsShowTitleBar)
__REG_GET_UI_DATA(bool, IsShowCollapse, false) //  ��������
__REG_SET_UI_DATA(bool, IsShowCollapse)
__REG_GET_UI_DATA(bool, IsShowScrollBar, false) //  �й�����
__REG_SET_UI_DATA(bool, IsShowScrollBar)

__REG_GET_UI_DATA(std::string, Name, std::string()) //  ����
__REG_SET_UI_DATA(std::string, Name)
__REG_GET_UI_DATA(std::string, Tips, std::string()) //  ��ʾ
__REG_SET_UI_DATA(std::string, Tips)
__REG_GET_UI_DATA(std::string, LSkin, std::string()) //  Ƥ��
__REG_SET_UI_DATA(std::string, LSkin)
__REG_GET_UI_DATA(std::string, Title, std::string()) //  ����
__REG_SET_UI_DATA(std::string, Title)

__REG_GET_UI_DATA(glm::vec4, Move, glm::vec4()) //  ��λ
__REG_SET_UI_DATA(glm::vec4, Move)
__REG_GET_UI_DATA(glm::vec4, Color0, glm::vec4()) //  ��ɫ
__REG_SET_UI_DATA(glm::vec4, Color0)
__REG_GET_UI_DATA(glm::vec4, _Move, glm::vec4()) //  ��λ
__REG_SET_UI_DATA(glm::vec4, _Move)

__REG_GET_UI_DATA(glm::vec2, StretchMin, glm::vec2(LAYOUT_STRETCH_BORDER * 3, LAYOUT_STRETCH_BORDER * 3)) //  ��������С����
__REG_SET_UI_DATA(glm::vec2, StretchMin)

__REG_GET_UI_DATA(std::vector<int>, Hotkeys, std::vector<int>())  //  �ȼ���Ӧ
__REG_SET_UI_DATA(std::vector<int>, Hotkeys)

inline void ParseUIData(CustomData & data, const std::string & key, const std::string & val)
{
    __REG_PARSE_UI_DATA(data, key, val, int, Align);
    __REG_PARSE_UI_DATA(data, key, val, float, BorderNumber);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsMulti);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsWindow);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsButton);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsVisible);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsShowNav);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsTextBox);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsEditBox);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsCanMove);
    __REG_PARSE_UI_DATA(data, key, val, bool, EnabledKey);
    __REG_PARSE_UI_DATA(data, key, val, bool, EnabledMouse);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsCanStretch);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsFullScreen);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsShowBorder);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsShowTitleBar);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsShowCollapse);
    __REG_PARSE_UI_DATA(data, key, val, bool, IsShowScrollBar);
    __REG_PARSE_UI_DATA(data, key, val, std::string, Name);
    __REG_PARSE_UI_DATA(data, key, val, std::string, Tips);
    __REG_PARSE_UI_DATA(data, key, val, std::string, LSkin);
    __REG_PARSE_UI_DATA(data, key, val, std::string, Title);
    __REG_PARSE_UI_DATA(data, key, val, glm::vec2, StretchMin);
    __REG_PARSE_UI_DATA(data, key, val, glm::vec4, Move);
    __REG_PARSE_UI_DATA(data, key, val, glm::vec4, Color0);
    __REG_PARSE_UI_DATA(data, key, val, std::vector<int>, Hotkey);

}

#define GetUIData(data, K)         __GetData##K(data)
#define SetUIData(data, K, val)    __SetData##K(data, val)