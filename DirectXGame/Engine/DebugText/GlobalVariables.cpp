#include "GlobalVariables.h"
#include "ImGuiManager.h"
#include <assert.h>
#include <json.hpp>
#include <fstream>
#include <Windows.h>
#include <string.h>

using namespace nlohmann;

GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables instance;

	return &instance;
}

void GlobalVariables::CreateData(const std::string& dataName) {
	files_[dataName];
}

void GlobalVariables::CreateGroup(const std::string& dataName, const std::string& groupName) {
	//指定名のオブジェクトがなければ追加する
	files_[dataName].datas_[groupName];
}

void GlobalVariables::Update() {
#ifdef _DEBUG
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
}
	if (!ImGui::BeginMenuBar()) { return; }

	for (std::map<std::string, Data>::iterator itFile = files_.begin(); itFile != files_.end(); ++itFile) {
		//データ名を取得
		const std::string& dataName = itFile->first;
		//データの参照を取得
		Data& data = itFile->second;
		if (!ImGui::BeginMenu(dataName.c_str())) {
			continue;
		}

		for (std::map<std::string, Group>::iterator itGroup = data.datas_.begin(); itGroup != data.datas_.end(); ++itGroup) {
			//グループ名を取得
			const std::string& groupName = itGroup->first;
			//グループの参照を取得
			Group& group = itGroup->second;
			
			if (ImGui::TreeNode(groupName.c_str())) {
				for (std::map<std::string, Item>::iterator itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {
					//項目名を取得
					const std::string& itemName = itItem->first;
					//項目の参照を取得
					Item& item = itItem->second;
					//int32_t型の値を保持していれば
					if (std::holds_alternative<int32_t>(item.value)) {
						int32_t* ptr = std::get_if<int32_t>(&item.value);
						ImGui::InputInt(itemName.c_str(), ptr, 1);
						//float型の値を保持していれば
					}
					else if (std::holds_alternative<float>(item.value)) {
						float* ptr = std::get_if<float>(&item.value);
						ImGui::DragFloat(itemName.c_str(), ptr, 0.01f);
						//Vec3型の値を保持していれば
					}
					else if (std::holds_alternative<Vector2>(item.value)) {
						Vector2* ptr = std::get_if<Vector2>(&item.value);
						ImGui::DragFloat2(itemName.c_str(), reinterpret_cast<float*>(ptr), 1.0f);
					}
					else if (std::holds_alternative<std::string>(item.value)) {
						std::string* ptr = std::get_if<std::string>(&item.value);
						char buff[256]{};
						if (ptr) {
							const char* str = ptr->c_str();
							strncpy_s(buff, str, _TRUNCATE);
						}
						ImGui::InputText(itemName.c_str(), buff, sizeof(buff));
						*ptr = std::string(buff);
					}
				}

				ImGui::TreePop();
			}
		}
		//改行
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {

			SaveFile(dataName);
			std::string message = std::format("{}.json saved.", dataName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}

	


	ImGui::EndMenuBar();
	ImGui::End();
#endif // _DEBUG
}

//void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
//	//グループの参照を取得
//	Group& group = datas_[groupName];
//	//新しい項目のデータを設定
//	Item newItem{};
//	newItem.value = value;
//	//設定した項目をstd::mapに追加
//	group.items[key] = newItem;
//
//}

//void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
//	// グループの参照を取得
//	Group& group = datas_[groupName];
//	// 新しい項目のデータを設定
//	Item newItem{};
//	newItem.value = value;
//	// 設定した項目をstd::mapに追加
//	group.items[key] = newItem;
//
//}

//void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value) {
//	// グループの参照を取得
//	Group& group = datas_[groupName];
//	// 新しい項目のデータを設定
//	Item newItem{};
//	newItem.value = value;
//	// 設定した項目をstd::mapに追加
//	group.items[key] = newItem;
//
//}

//void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const std::string& value) {
//	// グループの参照を取得
//	Group& group = datas_[groupName];
//	// 新しい項目のデータを設定
//	Item newItem{};
//	newItem.value = value;
//	// 設定した項目をstd::mapに追加
//	group.items[key] = newItem;
//
//}

template <typename Value>
void GlobalVariables::SetValue(const std::string& dataName, const std::string& groupName, const std::string& key, Value value) {
	// グループの参照を取得
	Group& group = files_[dataName].datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

template void GlobalVariables::SetValue<const std::string&>(const std::string& dataName, const std::string& groupName, const std::string& key, const std::string& value);
template void GlobalVariables::SetValue<const Vector2&>(const std::string& dataName, const std::string& groupName, const std::string& key, const Vector2& value);
template void GlobalVariables::SetValue<int32_t>(const std::string& dataName, const std::string& groupName, const std::string& key, int32_t value);
template void GlobalVariables::SetValue<float>(const std::string& dataName, const std::string& groupName, const std::string& key, float value);


void GlobalVariables::SaveFile(const std::string& dataName) {

	//データを検索
	std::map<std::string, Data>::iterator itData = files_.find(dataName);
	//未登録チェック
	assert(itData != files_.end());

	json root;

	root = json::object();
	//jsonオブジェクト登録
	root[dataName] = json::object();

	json groupsJson = json::array();

	//各グループについて
	for (std::map<std::string, Group>::iterator itGroup = itData->second.datas_.begin();
		itGroup != itData->second.datas_.end(); ++itGroup) {
		//グループ名を取得
		const std::string& groupName = itGroup->first;

		json groupJson;
		groupJson[groupName] = json::object();

		//各項目について
		for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
			itItem != itGroup->second.items.end(); ++itItem) {
			//項目名を取得
			const std::string& itemName = itItem->first;
			//項目の参照を取得
			Item& item = itItem->second;

			//int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item.value)) {
				//int32_t型の値を登録
				groupJson[groupName][itemName] = std::get<int32_t>(item.value);
				//float型の値を保持していれば
			}
			else if (std::holds_alternative<float>(item.value)) {
				//float型の値を登録
				groupJson[groupName][itemName] = std::get<float>(item.value);
				//Vec2型の値を保持していれば
			}
			else if (std::holds_alternative<Vector2>(item.value)) {
				//Vec2型の値を登録
				Vector2 value = std::get<Vector2>(item.value);
				groupJson[groupName][itemName] = json::array({ value.x, value.y });
			}
			else if (std::holds_alternative<std::string>(item.value)) {
				//std::string型に値を登録
				groupJson[groupName][itemName] = std::get<std::string>(item.value);
			}
		}
		groupsJson.push_back(groupJson);
	}

	root[dataName] = groupsJson;

	//ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::filesystem::create_directory(kDirectoryPath);
	}
	//書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + dataName + ".json";
	//書き込む用のファイルストリーム
	std::ofstream ofs;
	//ファイルを書き込み用に開く
	ofs.open(filePath);

	//ファイルオープン失敗？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	//ファイルにjsonの文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();

}

void GlobalVariables::LoadFiles() {

	std::string DirectoryPath = "Resources/GlobalVariables/";
	//ディレクトリがなければスキップする
	if (!std::filesystem::exists(DirectoryPath)) {
		return;
	}

	std::filesystem::directory_iterator dir_it(DirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {

		//ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		//ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		LoadFile(filePath.stem().string());
	}
}

void GlobalVariables::LoadFile(const std::string& dataName) {
	//読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + dataName + ".json";
	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);

	//ファイルオープン失敗？
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();

	//グループを検索
	auto itData = root.find(dataName);

	//未登録チェック
	assert(itData != root.end());

	// データを読み込む
    const json& data = itData.value(); // 見つかったデータ部分を取得
    for (const auto& d : data.items()) {
        //const std::string& groupName = group.key(); // グループ名を取得
        for (const auto& group : d.value().items()) {
            const std::string& groupName = group.key(); // アイテム名を取得

			for (const auto& item : group.value().items()) {
				const std::string& itemName = item.key();
				const json& value = item.value(); // 値を取得

				// 値の型に応じて処理を行う
				if (value.is_string()) {
					SetValue<const std::string&>(dataName, groupName, itemName, value.get<std::string>());
				}
				else if (value.is_number_integer()) {
					SetValue<int32_t>(dataName, groupName, itemName, value.get<int32_t>());
				}
				else if (value.is_number_float()) {
					SetValue<float>(dataName, groupName, itemName, value.get<float>());
				}
				else if (value.is_array() && value.size() == 2) {
					SetValue<const Vector2&>(dataName, groupName, itemName, Vector2{ value.at(0), value.at(1) });
				}
			}
            
        }
    }

}

//void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {
//
//	auto group = datas_.find(groupName);
//
//	if (group == datas_.end()) {
//		return;
//	}
//
//	if (group->second.items.find(key) == group->second.items.end()) {
//		SetValue(groupName, key, value);
//	}
//}

//void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {
//	auto group = datas_.find(groupName);
//
//	if (group == datas_.end()) {
//		return;
//	}
//
//	if (group->second.items.find(key) == group->second.items.end()) {
//		SetValue(groupName, key, value);
//	}
//}

//void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value) {
//	auto group = datas_.find(groupName);
//
//	if (group == datas_.end()) {
//		return;
//	}
//
//	if (group->second.items.find(key) == group->second.items.end()) {
//		SetValue(groupName, key, value);
//	}
//}

template <typename Value>
void GlobalVariables::AddItem(const std::string& dataName, const std::string& groupName, const std::string& key, Value value){

	auto itData = files_.find(dataName);
	assert(itData != files_.end());

	std::map<std::string, Group> datas = itData->second.datas_;
	std::map<std::string, Group>::iterator itGroup = datas.find(groupName);
	assert(itGroup != datas.end());

	if (itGroup == datas.end()) { return; }

	if (itGroup->second.items.find(key) == itGroup->second.items.end()) {
		SetValue<Value>(dataName, groupName, key, value);
	}

}
template void GlobalVariables::AddItem<const std::string&>(const std::string& dataName, const std::string& groupName, const std::string& key,const std::string& value);
template void GlobalVariables::AddItem<const Vector2&>(const std::string& dataName, const std::string& groupName, const std::string& key, const Vector2& value);
template void GlobalVariables::AddItem<int32_t>(const std::string& dataName, const std::string& groupName, const std::string& key, int32_t value);
template void GlobalVariables::AddItem<float>(const std::string& dataName, const std::string& groupName, const std::string& key, float value);


//int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const {
//
//
//	assert(datas_.find(groupName) != datas_.end());
//
//	const Group& group = datas_.at(groupName);
//
//	assert(group.items.find(key) != group.items.end());
//
//	return std::get<int>(group.items.find(key)->second.value);
//
//}

//float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const {
//
//	assert(datas_.find(groupName) != datas_.end());
//
//	const Group& group = datas_.at(groupName);
//
//	assert(group.items.find(key) != group.items.end());
//
//	return std::get<float>(group.items.find(key)->second.value);
//}

//Vector3 GlobalVariables::GetVec3Value(const std::string& groupName, const std::string& key) const {
//
//
//	assert(datas_.find(groupName) != datas_.end());
//
//	const Group& group = datas_.at(groupName);
//
//	assert(group.items.find(key) != group.items.end());
//
//	return std::get<Vector3>(group.items.find(key)->second.value);
//}

template <typename Value>
Value GlobalVariables::GetValue(const std::string& dataName, const std::string& groupName, const std::string& key) const {

	auto itData = files_.find(dataName);
	assert(itData != files_.end());

	std::map<std::string, Group> datas = itData->second.datas_;
	std::map<std::string, Group>::iterator itGroup = datas.find(groupName);
	assert(itGroup != datas.end());

	const Group& group = itGroup->second;

	assert(group.items.find(key) != group.items.end());

	return std::get<Value>(group.items.find(key)->second.value);
}

template std::string GlobalVariables::GetValue<std::string>(const std::string& dataName, const std::string& groupName, const std::string& key) const;
template Vector2 GlobalVariables::GetValue<Vector2>(const std::string& dataName, const std::string& groupName, const std::string& key) const;
template int32_t GlobalVariables::GetValue<int32_t>(const std::string& dataName, const std::string& groupName, const std::string& key) const;
template float GlobalVariables::GetValue<float>(const std::string& dataName, const std::string& groupName, const std::string& key) const;

std::string GlobalVariables::GetGroupName(const std::string& dataName, size_t index) {

	std::map<std::string, Data>::iterator itData = files_.find(dataName);
	assert(itData != files_.end());

	const Data& data = itData->second;
	assert(index < data.datas_.size());

	auto itGroup = data.datas_.begin();
	std::advance(itGroup, index);
	return itGroup->first;
}

