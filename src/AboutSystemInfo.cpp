#include "AboutSystemInfo.h"
const std::string ID = ("OS_INFO");
const std::string display_name = ("System INFO");
AboutSystemInfo::AboutSystemInfo(BasicRequiredInfo baseInfo) : DesktopicoProgram(static_ID, display_name)
{
  index = 0;
  FORMAT_PRIORITY = {
      KEY_VALUE_LIST_SIMPLE,
      KEY_VALUE_LIST_INDEXED};
  auto OSNAME = std::pair<std::string, std::string>(std::string("OS name"), baseInfo.OS_NAME);
  auto OSVER = std::pair<std::string, std::string>(std::string("Version"), std::to_string(baseInfo.version));
  auto AUTH = std::pair<std::string, std::string>(std::string("Author"), baseInfo.author_name);
  return_data.OPTIONS_MAP.insert(OSNAME);
  return_data.OPTIONS_MAP.insert(OSVER);
  return_data.OPTIONS_MAP.insert(AUTH);
  return_data.OPTIONS_VECTOR.push_back(OSNAME);
  return_data.OPTIONS_VECTOR.push_back(OSVER);
  return_data.OPTIONS_VECTOR.push_back(AUTH);
  systemDetails.emplace(std::string("Version"), std::to_string(baseInfo.version));

  systemDetails.emplace(std::string("Author"), baseInfo.author_name);
  return_data.INDEX = &index;
  return_data.OPTIONS_MAP = systemDetails;
}

void AboutSystemInfo::init()
{
  return_button_funct = [this]()
  { previous(); };
  select_button_funct = [this]()
  { select(); };
  next_button_funct = [this]()
  { next(); };
  ProgramDefinedButtons.RETURN.setCallbackFunction(return_button_funct);
  ProgramDefinedButtons.SELECT.setCallbackFunction(select_button_funct);
  ProgramDefinedButtons.NEXT.setCallbackFunction(next_button_funct);
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;
  returnValue.data = &return_data;
}

ProgramReturn *AboutSystemInfo::run(bool *availableButtons)
{
  // previous();

  returnValue.data = &return_data;
  return &returnValue;
}

void AboutSystemInfo::select()
{
}

void AboutSystemInfo::previous()
{
  index--;
}

void AboutSystemInfo::next()
{
  index++;
}
