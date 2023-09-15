#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"


namespace Dpr::Message {
    struct MessageManager : ILClass<MessageManager> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields{
            void* msbtLoader; //Dpr_Message_MsgDataFileLoader_o*
            void* dataModel; //Dpr_Message_MessageDataModel_o*
            void* tagWordBuffer; //Dpr_Message_TagWordBuffer_o*
            void* wordDataPool; //Dpr_Message_MessageWordDataPool_o*
        };

        static inline Dpr::Message::MessageManager::Object* instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$MessageManager$$get_Instance);
        }

        inline System::String::Object * GetSimpleMessage(System::String::Object *fileName,System::String::Object *label) {
            return external<System::String::Object *>(0x0210d000, this, fileName, label);
        }

        inline System::String::Object * GetNameMessage(System::String::Object *fileName,System::String::Object *label) {
            return external<System::String::Object *>(0x0210ce50, this, fileName, label);
        }
    };
}

