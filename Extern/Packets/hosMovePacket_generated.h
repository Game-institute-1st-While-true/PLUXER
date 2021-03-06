// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_HOSMOVEPACKET_HOS_PACKET_H_
#define FLATBUFFERS_GENERATED_HOSMOVEPACKET_HOS_PACKET_H_

#include "flatbuffers/flatbuffers.h"

#include "hosPacketData_generated.h"

namespace hos {
namespace Packet {

struct MoveProtocol;
struct MoveProtocolBuilder;

struct MoveProtocol FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MoveProtocolBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_FOWARD = 6,
    VT_ROTATION = 8,
    VT_DIRECTION = 10,
    VT_DASH = 12
  };
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  const hos::Packet::Vec3 *foward() const {
    return GetStruct<const hos::Packet::Vec3 *>(VT_FOWARD);
  }
  const hos::Packet::Vec3 *rotation() const {
    return GetStruct<const hos::Packet::Vec3 *>(VT_ROTATION);
  }
  hos::Packet::MOVE_DIR direction() const {
    return static_cast<hos::Packet::MOVE_DIR>(GetField<int8_t>(VT_DIRECTION, 0));
  }
  bool dash() const {
    return GetField<uint8_t>(VT_DASH, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<hos::Packet::Vec3>(verifier, VT_FOWARD) &&
           VerifyField<hos::Packet::Vec3>(verifier, VT_ROTATION) &&
           VerifyField<int8_t>(verifier, VT_DIRECTION) &&
           VerifyField<uint8_t>(verifier, VT_DASH) &&
           verifier.EndTable();
  }
};

struct MoveProtocolBuilder {
  typedef MoveProtocol Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(MoveProtocol::VT_ID, id, 0);
  }
  void add_foward(const hos::Packet::Vec3 *foward) {
    fbb_.AddStruct(MoveProtocol::VT_FOWARD, foward);
  }
  void add_rotation(const hos::Packet::Vec3 *rotation) {
    fbb_.AddStruct(MoveProtocol::VT_ROTATION, rotation);
  }
  void add_direction(hos::Packet::MOVE_DIR direction) {
    fbb_.AddElement<int8_t>(MoveProtocol::VT_DIRECTION, static_cast<int8_t>(direction), 0);
  }
  void add_dash(bool dash) {
    fbb_.AddElement<uint8_t>(MoveProtocol::VT_DASH, static_cast<uint8_t>(dash), 0);
  }
  explicit MoveProtocolBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MoveProtocolBuilder &operator=(const MoveProtocolBuilder &);
  flatbuffers::Offset<MoveProtocol> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MoveProtocol>(end);
    return o;
  }
};

inline flatbuffers::Offset<MoveProtocol> CreateMoveProtocol(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    const hos::Packet::Vec3 *foward = 0,
    const hos::Packet::Vec3 *rotation = 0,
    hos::Packet::MOVE_DIR direction = hos::Packet::MOVE_DIR_N,
    bool dash = false) {
  MoveProtocolBuilder builder_(_fbb);
  builder_.add_rotation(rotation);
  builder_.add_foward(foward);
  builder_.add_id(id);
  builder_.add_dash(dash);
  builder_.add_direction(direction);
  return builder_.Finish();
}

inline const hos::Packet::MoveProtocol *GetMoveProtocol(const void *buf) {
  return flatbuffers::GetRoot<hos::Packet::MoveProtocol>(buf);
}

inline const hos::Packet::MoveProtocol *GetSizePrefixedMoveProtocol(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<hos::Packet::MoveProtocol>(buf);
}

inline bool VerifyMoveProtocolBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<hos::Packet::MoveProtocol>(nullptr);
}

inline bool VerifySizePrefixedMoveProtocolBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<hos::Packet::MoveProtocol>(nullptr);
}

inline void FinishMoveProtocolBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<hos::Packet::MoveProtocol> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMoveProtocolBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<hos::Packet::MoveProtocol> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Packet
}  // namespace hos

#endif  // FLATBUFFERS_GENERATED_HOSMOVEPACKET_HOS_PACKET_H_
