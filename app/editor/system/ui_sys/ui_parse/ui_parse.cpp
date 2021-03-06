#include "ui_parse.h"
#include "../ui_state/ui_state.h"
#include "../ui_object/ui_object.h"
#include "../../editor_sys/delegate/main/ui_delegate_main.h"
#include "../../editor_sys/delegate/explorer/ui_delegate_explorer.h"

// ...
//  UI�¼�ί��ӳ���
// ...
static std::map<std::string, UIEvent::DelegateHandler *(*)()> s_DelegateMap = {
    std::make_pair("editor/editor/delegate/main/uievent_delegate_main_stage", []() { return (UIEvent::DelegateHandler *)new UIDelegateMainStage(); }),
    std::make_pair("editor/editor/delegate/main/uievent_delegate_main_global", []() { return (UIEvent::DelegateHandler *)new UIDelegateMainGlobal(); }),
    std::make_pair("editor/editor/delegate/main/uievent_delegate_main_obj_list", []() { return (UIEvent::DelegateHandler *)new UIDelegateMainObjList(); }),
    std::make_pair("editor/editor/delegate/main/uievent_delegate_main_res_list", []() { return (UIEvent::DelegateHandler *)new UIDelegateMainResList(); }),
    std::make_pair("editor/editor/delegate/main/uievent_delegate_main_com_list", []() { return (UIEvent::DelegateHandler *)new UIDelegateMainComList(); }),
    std::make_pair("editor/editor/delegate/explorer/ui_delegate_explorer", []() { return (UIEvent::DelegateHandler *)new UIDelegateExplorer(); }),
};

SharePtr<UIObject> UIParser::CreateObject(const int type)
{
    switch ((UITypeEnum)type)
    {
    case UITypeEnum::kOther: return std::create_ptr<UIObject>();
    case UITypeEnum::kLayout: return std::create_ptr<UIObjectLayout>();
    case UITypeEnum::kTreeBox: return std::create_ptr<UIObjectTreeBox>();
    case UITypeEnum::kTextBox: return std::create_ptr<UIObjectTextBox>();
    case UITypeEnum::kImageBox: return std::create_ptr<UIObjectImageBox>();
    case UITypeEnum::kComboBox: return std::create_ptr<UIObjectComboBox>();
    case UITypeEnum::kUICanvas: return std::create_ptr<UIObjectUICanvas>();
    case UITypeEnum::kGLCanvas: return std::create_ptr<UIObjectGLCanvas>();
    }
    ASSERT_LOG(false, "Error Type: {0}!", type);
    return nullptr;
}

SharePtr<UIObject> UIParser::Parse(const std::string & url)
{
    auto json = mmc::Json::FromFile(url);
    ASSERT_LOG(json, "Parse: {0}", url);
    return Parse(json);
}

SharePtr<UIObject> UIParser::Parse(const mmc::Json::Pointer json)
{
    auto object = CreateObject(std::stoi(json->At("__Property", "Type")->ToString()));
    ASSERT_LOG(object != nullptr, "");
    Parse__Property(json, object);
    Parse__Children(json, object);
    return object;
}

void UIParser::Parse__Property(const mmc::Json::Pointer json, const SharePtr<UIObject> & object)
{
    for (auto ele : json->At("__Property"))
    {
        ASSERT_LOG(ele.mVal->GetType() == mmc::Json::Type::kSTRING, "{0}", ele.mKey);
        if (ele.mKey == "EventDelegate")
        {
            ASSERT_LOG(s_DelegateMap.count(ele.mVal->ToString()) == 1, ele.mVal->ToString().c_str());
            object->BindDelegate(s_DelegateMap.at(ele.mVal->ToString())());
        }
        else
        {
            object->GetState()->FromStringParse(ele.mKey, ele.mVal->ToString());
        }
    }
}

void UIParser::Parse__Children(const mmc::Json::Pointer json, const SharePtr<UIObject> & object)
{
    for (auto ele : json->At("__Children"))
    {
        object->InsertObject(Parse(ele.mVal));
    }
}

bool UIParser::FromStringParse(StringValueTypeEnum type, const std::string & val, const std::string & sep, void * out)
{
   switch (type)
    {
    case StringValueTypeEnum::kInt:
        {
            *((int *)out) = std::stoi(val);
        }
        return true;
    case StringValueTypeEnum::kBool:
        {
            *((bool *)out) = val.at(0) == 't'
                          && val.at(1) == 'r'
                          && val.at(2) == 'u'
                          && val.at(3) == 'e';
        }
        return true;
    case StringValueTypeEnum::kFloat:
        {
            *((float *)out) = std::stof(val);
        }
        return true;
    case StringValueTypeEnum::kAsset:
    case StringValueTypeEnum::kString:
        {
            *((std::string *)out) = val;
        }
        return true;
    case StringValueTypeEnum::kVector2:
        {
            auto value = (glm::vec2 *)out;
            auto array = tools::Split(val, sep);
            value->x = std::stof(array.at(0));
            value->y = std::stof(array.at(1));
        }
        return true;
    case StringValueTypeEnum::kVector3:
        {
            auto value = (glm::vec3 *)out;
            auto array = tools::Split(val, sep);
            value->x = std::stof(array.at(0));
            value->y = std::stof(array.at(1));
            value->z = std::stof(array.at(2));
        }
        return true;
    case StringValueTypeEnum::kColor4:
    case StringValueTypeEnum::kVector4:
        {
            auto value = (glm::vec4 *)out;
            auto array = tools::Split(val, sep);
            value->x = std::stof(array.at(0));
            value->y = std::stof(array.at(1));
            value->z = std::stof(array.at(2));
            value->w = std::stof(array.at(3));
        }
        return true;
    case StringValueTypeEnum::kIntList:
        {
            auto value = (std::vector<int>*)out;
            auto array = tools::Split(val, sep);
            std::transform(
                array.begin(), array.end(),
                std::back_inserter(*value),
                [](const std::string & val)
                { return std::stoi(val); });
        }
        return true;
    case StringValueTypeEnum::kFloatList:
        {
            auto value = (std::vector<float> *)out;
            auto array = tools::Split(val, sep);
            std::transform(
                array.begin(), array.end(),
                std::back_inserter(*value),
                [](const std::string & val)
                { return std::stof(val); });
        }
        return true;
    case StringValueTypeEnum::kStringList:
        {
            auto value = (std::vector<std::string> *)out;
            auto array = tools::Split(val, sep);
            *value = std::move(array);
        }
        return true;
    }
    return false;
}
