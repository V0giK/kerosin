#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "ModelParameters.h"
#include <lvgl.h>

bool loadModel(int id, ModelParameters &model);
bool saveModel(int id, ModelParameters &model);
void deleteModel(int id);
void loadModelsFromStorage();
lv_obj_t* addModelButton2container(String name, int id);
lv_obj_t* addModelPlusButton2container(int id);
bool loadModelParameters(const String &filename, StaticJsonDocument<1024> &doc);
void btnModelSaveClick(lv_obj_t *objLoadedModel, lv_obj_t *objModelPlus, ModelParameters &model);
bool sendModelDataToController(const ModelParameters &model);
void viewModelParameters(TankTypeEnum tankType);
