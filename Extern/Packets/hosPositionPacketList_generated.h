// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_HOSPOSITIONPACKETLIST_HOS_PACKET_H_
#define FLATBUFFERS_GENERATED_HOSPOSITIONPACKETLIST_HOS_PACKET_H_

#include "flatbuffers/flatbuffers.h"

#include "hosPacketData_generated.h"
#include "hosPositionPacket_generated.h"

namespace hos {
namespace Packet {

struct PositionListProtocol;
struct PositionListProtocolBuilder;

struct PositionListProtocol FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef PositionListProtocolBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_POSITIONS = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<hos::Packet::PositionProtocol>> *Positions() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<hos::Packet::PositionProtocol>> *>(VT_POSITIONS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_POSITIONS) &&
           verifier.VerifyVector(Positions()) &&
           verifier.VerifyVectorOfTables(Positions()) &&
           verifier.EndTable();
  }
};

struct PositionListProtocolBuilder {
  typedef PositionListProtocol Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_Positions(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hos::Packet::PositionProtocol>>> Positions) {
    fbb_.AddOffset(PositionListProtocol::VT_POSITIONS, Positions);
  }
  explicit PositionListProtocolBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  PositionListProtocolBuilder &operator=(const PositionListProtocolBuilder &);
  flatbuffers::Offset<PositionListProtocol> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<PositionListProtocol>(end);
    return o;
  }
};

inline flatbuffers::Offset<PositionListProtocol> CreatePositionListProtocol(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<hos::Packet::PositionProtocol>>> Positions = 0) {
  PositionListProtocolBuilder builder_(_fbb);
  builder_.add_Positions(Positions);
  return builder_.Finish();
}

inline flatbuffers::Offset<PositionListProtocol> CreatePositionListProtocolDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<hos::Packet::PositionProtocol>> *Positions = nullptr) {
  auto Positions__ = Positions ? _fbb.CreateVector<flatbuffers::Offset<hos::Packet::PositionProtocol>>(*Positions) : 0;
  return hos::Packet::CreatePositionListProtocol(
      _fbb,
      Positions__);
}

inline const hos::Packet::PositionListProtocol *GetPositionListProtocol(const void *buf) {
  return flatbuffers::GetRoot<hos::Packet::PositionListProtocol>(buf);
}

inline const hos::Packet::PositionListProtocol *GetSizePrefixedPositionListProtocol(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<hos::Packet::PositionListProtocol>(buf);
}

inline bool VerifyPositionListProtocolBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<hos::Packet::PositionListProtocol>(nullptr);
}

inline bool VerifySizePrefixedPositionListProtocolBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<hos::Packet::PositionListProtocol>(nullptr);
}

inline void FinishPositionListProtocolBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<hos::Packet::PositionListProtocol> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedPositionListProtocolBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<hos::Packet::PositionListProtocol> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Packet
}  // namespace hos

#endif  // FLATBUFFERS_GENERATED_HOSPOSITIONPACKETLIST_HOS_PACKET_H_
