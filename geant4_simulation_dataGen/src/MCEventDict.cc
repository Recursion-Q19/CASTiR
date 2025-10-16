// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIsayyeddIDesktopdICASTiRdIgeant4_simulation_dataGendIsrcdIMCEventDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/home/sayyed/Desktop/CASTiR/geant4_simulation_dataGen/include/MCEvent.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_opticalHit(void *p = nullptr);
   static void *newArray_opticalHit(Long_t size, void *p);
   static void delete_opticalHit(void *p);
   static void deleteArray_opticalHit(void *p);
   static void destruct_opticalHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::opticalHit*)
   {
      ::opticalHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::opticalHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("opticalHit", ::opticalHit::Class_Version(), "", 10,
                  typeid(::opticalHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::opticalHit::Dictionary, isa_proxy, 4,
                  sizeof(::opticalHit) );
      instance.SetNew(&new_opticalHit);
      instance.SetNewArray(&newArray_opticalHit);
      instance.SetDelete(&delete_opticalHit);
      instance.SetDeleteArray(&deleteArray_opticalHit);
      instance.SetDestructor(&destruct_opticalHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::opticalHit*)
   {
      return GenerateInitInstanceLocal(static_cast<::opticalHit*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::opticalHit*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_EventPrimary(void *p = nullptr);
   static void *newArray_EventPrimary(Long_t size, void *p);
   static void delete_EventPrimary(void *p);
   static void deleteArray_EventPrimary(void *p);
   static void destruct_EventPrimary(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EventPrimary*)
   {
      ::EventPrimary *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EventPrimary >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("EventPrimary", ::EventPrimary::Class_Version(), "", 62,
                  typeid(::EventPrimary), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::EventPrimary::Dictionary, isa_proxy, 4,
                  sizeof(::EventPrimary) );
      instance.SetNew(&new_EventPrimary);
      instance.SetNewArray(&newArray_EventPrimary);
      instance.SetDelete(&delete_EventPrimary);
      instance.SetDeleteArray(&deleteArray_EventPrimary);
      instance.SetDestructor(&destruct_EventPrimary);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EventPrimary*)
   {
      return GenerateInitInstanceLocal(static_cast<::EventPrimary*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::EventPrimary*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MCEvent(void *p = nullptr);
   static void *newArray_MCEvent(Long_t size, void *p);
   static void delete_MCEvent(void *p);
   static void deleteArray_MCEvent(void *p);
   static void destruct_MCEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MCEvent*)
   {
      ::MCEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MCEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("MCEvent", ::MCEvent::Class_Version(), "", 112,
                  typeid(::MCEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MCEvent::Dictionary, isa_proxy, 4,
                  sizeof(::MCEvent) );
      instance.SetNew(&new_MCEvent);
      instance.SetNewArray(&newArray_MCEvent);
      instance.SetDelete(&delete_MCEvent);
      instance.SetDeleteArray(&deleteArray_MCEvent);
      instance.SetDestructor(&destruct_MCEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MCEvent*)
   {
      return GenerateInitInstanceLocal(static_cast<::MCEvent*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::MCEvent*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr opticalHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *opticalHit::Class_Name()
{
   return "opticalHit";
}

//______________________________________________________________________________
const char *opticalHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::opticalHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int opticalHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::opticalHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *opticalHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::opticalHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *opticalHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::opticalHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr EventPrimary::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *EventPrimary::Class_Name()
{
   return "EventPrimary";
}

//______________________________________________________________________________
const char *EventPrimary::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventPrimary*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int EventPrimary::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventPrimary*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EventPrimary::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventPrimary*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EventPrimary::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventPrimary*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MCEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MCEvent::Class_Name()
{
   return "MCEvent";
}

//______________________________________________________________________________
const char *MCEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MCEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MCEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MCEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MCEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MCEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MCEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MCEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void opticalHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class opticalHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(opticalHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(opticalHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_opticalHit(void *p) {
      return  p ? new(p) ::opticalHit : new ::opticalHit;
   }
   static void *newArray_opticalHit(Long_t nElements, void *p) {
      return p ? new(p) ::opticalHit[nElements] : new ::opticalHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_opticalHit(void *p) {
      delete (static_cast<::opticalHit*>(p));
   }
   static void deleteArray_opticalHit(void *p) {
      delete [] (static_cast<::opticalHit*>(p));
   }
   static void destruct_opticalHit(void *p) {
      typedef ::opticalHit current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::opticalHit

//______________________________________________________________________________
void EventPrimary::Streamer(TBuffer &R__b)
{
   // Stream an object of class EventPrimary.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EventPrimary::Class(),this);
   } else {
      R__b.WriteClassBuffer(EventPrimary::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EventPrimary(void *p) {
      return  p ? new(p) ::EventPrimary : new ::EventPrimary;
   }
   static void *newArray_EventPrimary(Long_t nElements, void *p) {
      return p ? new(p) ::EventPrimary[nElements] : new ::EventPrimary[nElements];
   }
   // Wrapper around operator delete
   static void delete_EventPrimary(void *p) {
      delete (static_cast<::EventPrimary*>(p));
   }
   static void deleteArray_EventPrimary(void *p) {
      delete [] (static_cast<::EventPrimary*>(p));
   }
   static void destruct_EventPrimary(void *p) {
      typedef ::EventPrimary current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::EventPrimary

//______________________________________________________________________________
void MCEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class MCEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MCEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(MCEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MCEvent(void *p) {
      return  p ? new(p) ::MCEvent : new ::MCEvent;
   }
   static void *newArray_MCEvent(Long_t nElements, void *p) {
      return p ? new(p) ::MCEvent[nElements] : new ::MCEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_MCEvent(void *p) {
      delete (static_cast<::MCEvent*>(p));
   }
   static void deleteArray_MCEvent(void *p) {
      delete [] (static_cast<::MCEvent*>(p));
   }
   static void destruct_MCEvent(void *p) {
      typedef ::MCEvent current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::MCEvent

namespace ROOT {
   // Registration Schema evolution read functions
   int RecordReadRules_MCEventDict() {
      return 0;
   }
   static int _R__UNIQUE_DICT_(ReadRules_MCEventDict) = RecordReadRules_MCEventDict();R__UseDummy(_R__UNIQUE_DICT_(ReadRules_MCEventDict));
} // namespace ROOT
namespace {
  void TriggerDictionaryInitialization_MCEventDict_Impl() {
    static const char* headers[] = {
"/home/sayyed/Desktop/CASTiR/geant4_simulation_dataGen/include/MCEvent.hh",
nullptr
    };
    static const char* includePaths[] = {
"/usr/local/include/",
"/home/sayyed/Desktop/CASTiR/geant4_simulation_dataGen/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "MCEventDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$/home/sayyed/Desktop/CASTiR/geant4_simulation_dataGen/include/MCEvent.hh")))  opticalHit;
class __attribute__((annotate("$clingAutoload$/home/sayyed/Desktop/CASTiR/geant4_simulation_dataGen/include/MCEvent.hh")))  EventPrimary;
class __attribute__((annotate("$clingAutoload$/home/sayyed/Desktop/CASTiR/geant4_simulation_dataGen/include/MCEvent.hh")))  MCEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "MCEventDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/home/sayyed/Desktop/CASTiR/geant4_simulation_dataGen/include/MCEvent.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"EventPrimary", payloadCode, "@",
"MCEvent", payloadCode, "@",
"opticalHit", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("MCEventDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_MCEventDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_MCEventDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_MCEventDict() {
  TriggerDictionaryInitialization_MCEventDict_Impl();
}
