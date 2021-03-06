#pragma once
#include <vector>
#include <map>
#include <memory>
#include <all_far.h>
#include <fstdlib.h>

struct ELFInfo;

class PluginImpl
{
	std::string _name, _dir;
	std::shared_ptr<ELFInfo> _elf_info;
	char _panel_title[512];

	bool GetFindData_ROOT(FP_SizeItemList &il, int OpMode);
	bool GetFindData_DISASM(FP_SizeItemList &il, int OpMode);

  public:
	PluginImpl(const char *name, uint8_t bitness, uint8_t endianess);
	~PluginImpl();

	int GetFindData(PluginPanelItem **pPanelItem,int *pItemsNumber,int OpMode);
	void FreeFindData(PluginPanelItem *PanelItem,int ItemsNumber);
	int SetDirectory(const char *Dir,int OpMode);
	void GetOpenPluginInfo(struct OpenPluginInfo *Info);
	int DeleteFiles(struct PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
	int ProcessHostFile(struct PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
	int GetFiles(struct PluginPanelItem *PanelItem,int ItemsNumber,
		int Move,char *DestPath,int OpMode);
	int PutFiles(struct PluginPanelItem *PanelItem,int ItemsNumber,
		int Move,int OpMode);
	int ProcessKey(int Key,unsigned int ControlState);
};
