# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: heartbeat_svr/youme_heartbeat.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='heartbeat_svr/youme_heartbeat.proto',
  package='YOUMEServiceProtocol',
  syntax='proto2',
  serialized_pb=_b('\n#heartbeat_svr/youme_heartbeat.proto\x12\x14YOUMEServiceProtocol\"K\n\x0cHeartbeatReq\x12\x0f\n\x07version\x18\x01 \x02(\x05\x12\x10\n\x08tips_seq\x18\x02 \x01(\x04\x12\x18\n\x10weixin_token_seq\x18\x03 \x01(\x04\"_\n\x0cHeartbeatRsp\x12\x0f\n\x07version\x18\x01 \x02(\x05\x12\x0b\n\x03ret\x18\x02 \x02(\x05\x12\x11\n\tsync_flag\x18\x03 \x01(\x05\x12\x1e\n\x16weixin_token_sync_flag\x18\x04 \x01(\x05\x42\x02H\x03')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_HEARTBEATREQ = _descriptor.Descriptor(
  name='HeartbeatReq',
  full_name='YOUMEServiceProtocol.HeartbeatReq',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='version', full_name='YOUMEServiceProtocol.HeartbeatReq.version', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='tips_seq', full_name='YOUMEServiceProtocol.HeartbeatReq.tips_seq', index=1,
      number=2, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='weixin_token_seq', full_name='YOUMEServiceProtocol.HeartbeatReq.weixin_token_seq', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=61,
  serialized_end=136,
)


_HEARTBEATRSP = _descriptor.Descriptor(
  name='HeartbeatRsp',
  full_name='YOUMEServiceProtocol.HeartbeatRsp',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='version', full_name='YOUMEServiceProtocol.HeartbeatRsp.version', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='ret', full_name='YOUMEServiceProtocol.HeartbeatRsp.ret', index=1,
      number=2, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='sync_flag', full_name='YOUMEServiceProtocol.HeartbeatRsp.sync_flag', index=2,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='weixin_token_sync_flag', full_name='YOUMEServiceProtocol.HeartbeatRsp.weixin_token_sync_flag', index=3,
      number=4, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=138,
  serialized_end=233,
)

DESCRIPTOR.message_types_by_name['HeartbeatReq'] = _HEARTBEATREQ
DESCRIPTOR.message_types_by_name['HeartbeatRsp'] = _HEARTBEATRSP

HeartbeatReq = _reflection.GeneratedProtocolMessageType('HeartbeatReq', (_message.Message,), dict(
  DESCRIPTOR = _HEARTBEATREQ,
  __module__ = 'heartbeat_svr.youme_heartbeat_pb2'
  # @@protoc_insertion_point(class_scope:YOUMEServiceProtocol.HeartbeatReq)
  ))
_sym_db.RegisterMessage(HeartbeatReq)

HeartbeatRsp = _reflection.GeneratedProtocolMessageType('HeartbeatRsp', (_message.Message,), dict(
  DESCRIPTOR = _HEARTBEATRSP,
  __module__ = 'heartbeat_svr.youme_heartbeat_pb2'
  # @@protoc_insertion_point(class_scope:YOUMEServiceProtocol.HeartbeatRsp)
  ))
_sym_db.RegisterMessage(HeartbeatRsp)


DESCRIPTOR.has_options = True
DESCRIPTOR._options = _descriptor._ParseOptions(descriptor_pb2.FileOptions(), _b('H\003'))
# @@protoc_insertion_point(module_scope)
