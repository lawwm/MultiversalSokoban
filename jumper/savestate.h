#ifndef SAVESTATE_H
#define SAVESTATE_H

#include <iostream>
#include <sstream>

#include "tinyxml2.h"
#include "windowsize.h"

using namespace tinyxml2;

class SaveState {
public:
	
	SaveState() {};
	
	void parse(std::string overworldname, std::vector<CompletionSprite>& completionSprites, Graphics& graphics)
	{
		XMLDocument doc;
		std::stringstream ss;
		ss << overworldname << ".tmx";
		
		doc.LoadFile(ss.str().c_str());
		
		XMLElement* mapNode = doc.FirstChildElement("map");
		
		XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
		
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
				
			// if objectgroup is not chests, check next object group
			if (ss.str() != "chests") {
				pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
				continue;
			}

			// check each object
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			while (pObject) {
				float x, y, width, height;
				int levelNumber;
				bool isCompleted;
				x = pObject->FloatAttribute("x");
				y = pObject->FloatAttribute("y");
				width = pObject->FloatAttribute("width");
				height = pObject->FloatAttribute("height");
				levelNumber = pObject->IntAttribute("name");
				
				// check properties for whether the chest has been completed
				XMLElement* pProperties = pObject->FirstChildElement("properties");
				while (pProperties) {
					XMLElement* pProperty = pProperties->FirstChildElement("property");
					while (pProperty) {
						const char* name = pProperty->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "iscompleted") {
							isCompleted = pProperty->BoolAttribute("value");
						}
						pProperty = pProperty->NextSiblingElement("property");
					}
					pProperties = pProperties->NextSiblingElement("properties");
				}

				completionSprites.emplace_back(graphics, Vector2(x * Window::getSpriteScale(), y * Window::getSpriteScale()), levelNumber, isCompleted);
				
				pObject = pObject->NextSiblingElement("object");
			}

			// check next object group
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}

	}

	void update(std::string overworldname, const std::vector<CompletionSprite>& completionSprites) {
		XMLDocument doc;

		std::stringstream ss;
		ss << overworldname << ".tmx";

		doc.LoadFile(ss.str().c_str());

		XMLElement* mapNode = doc.FirstChildElement("map");

		XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");

		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;

			// if objectgroup is not chests, check next object group
			if (ss.str() != "chests") {
				pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
				continue;
			}

			// check each object
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			while (pObject) {
				int levelNumber = pObject->IntAttribute("name");
				bool isCompleted = false;
				for (auto& completionSprite : completionSprites) {
					if (completionSprite.getLevelNumber() == levelNumber) {
						isCompleted = completionSprite.getCompleted();
					}
				}

				// set property for the chest depending on the chest number
				XMLElement* pProperties = pObject->FirstChildElement("properties");
				while (pProperties) {
					XMLElement* pProperty = pProperties->FirstChildElement("property");
					while (pProperty) {
						const char* name = pProperty->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "iscompleted") {
							pProperty->SetAttribute("value", isCompleted);
						}
						pProperty = pProperty->NextSiblingElement("property");
					}
					pProperties = pProperties->NextSiblingElement("properties");
				}

				pObject = pObject->NextSiblingElement("object");
			}

			// check next object group
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
		
		doc.SaveFile(ss.str().c_str());
	}
};

#endif