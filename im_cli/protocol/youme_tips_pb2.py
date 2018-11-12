# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: tips_svr/youme_tips.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from comm import youme_comm_pb2 as comm_dot_youme__comm__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='tips_svr/youme_tips.proto',
  package='YOUMEServiceProtocol',
  syntax='proto2',
  serialized_pb=_b('\n\x19tips_svr/youme_tips.proto\x12\x14YOUMEServiceProtocol\x1a\x15\x63omm/youme_comm.proto\"\xb4\x02\n\x07\x41ppTips\x12\x0f\n\x07tips_id\x18\x01 \x02(\x04\x12\x12\n\nchannel_id\x18\x02 \x02(\x0c\x12\r\n\x05title\x18\x03 \x02(\x0c\x12\x11\n\ttips_type\x18\x04 \x02(\x05\x12\x11\n\tloop_type\x18\x05 \x02(\x05\x12\x0f\n\x07\x63ontent\x18\x06 \x02(\x0c\x12\x15\n\rlink_keywords\x18\x07 \x02(\x0c\x12\x11\n\tlink_addr\x18\x08 \x02(\x0c\x12\x14\n\x0cloop_weekday\x18\t \x02(\x0c\x12\x16\n\x0estart_datetime\x18\n \x02(\x0c\x12\x14\n\x0c\x65nd_datetime\x18\x0b \x02(\x0c\x12\x0e\n\x06\x65nable\x18\x0c \x02(\r\x12\x0e\n\x06old_id\x18\r \x02(\x04\x12\x30\n\x07\x63omment\x18\x0e \x01(\x0b\x32\x1f.YOUMECommonProtocol.MsgComment\"n\n\nGetTipsRsp\x12\x0f\n\x07version\x18\x01 \x02(\x05\x12\x0b\n\x03ret\x18\x02 \x02(\x05\x12\x10\n\x08tips_cnt\x18\x03 \x02(\x05\x12\x30\n\ttips_list\x18\x04 \x03(\x0b\x32\x1d.YOUMEServiceProtocol.AppTipsB\x02H\x03')
  ,
  dependencies=[comm_dot_youme__comm__pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_APPTIPS = _descriptor.Descriptor(
  name='AppTips',
  full_name='YOUMEServiceProtocol.AppTips',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='tips_id', full_name='YOUMEServiceProtocol.AppTips.tips_id', index=0,
      number=1, type=4, cpp_type=4, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='channel_id', full_name='YOUMEServiceProtocol.AppTips.channel_id', index=1,
      number=2, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='title', full_name='YOUMEServiceProtocol.AppTips.title', index=2,
      number=3, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='tips_type', full_name='YOUMEServiceProtocol.AppTips.tips_type', index=3,
      number=4, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='loop_type', full_name='YOUMEServiceProtocol.AppTips.loop_type', index=4,
      number=5, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='content', full_name='YOUMEServiceProtocol.AppTips.content', index=5,
      number=6, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='link_keywords', full_name='YOUMEServiceProtocol.AppTips.link_keywords', index=6,
      number=7, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='link_addr', full_name='YOUMEServiceProtocol.AppTips.link_addr', index=7,
      number=8, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='loop_weekday', full_name='YOUMEServiceProtocol.AppTips.loop_weekday', index=8,
      number=9, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='start_datetime', full_name='YOUMEServiceProtocol.AppTips.start_datetime', index=9,
      number=10, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='end_datetime', full_name='YOUMEServiceProtocol.AppTips.end_datetime', index=10,
      number=11, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='enable', full_name='YOUMEServiceProtocol.AppTips.enable', index=11,
      number=12, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='old_id', full_name='YOUMEServiceProtocol.AppTips.old_id', index=12,
      number=13, type=4, cpp_type=4, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='comment', full_name='YOUMEServiceProtocol.AppTips.comment', index=13,
      number=14, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
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
  serialized_start=75,
  serialized_end=383,
)


_GETTIPSRSP = _descriptor.Descriptor(
  name='GetTipsRsp',
  full_name='YOUMEServiceProtocol.GetTipsRsp',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='version', full_name='YOUMEServiceProtocol.GetTipsRsp.version', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='ret', full_name='YOUMEServiceProtocol.GetTipsRsp.ret', index=1,
      number=2, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='tips_cnt', full_name='YOUMEServiceProtocol.GetTipsRsp.tips_cnt', index=2,
      number=3, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='tips_list', full_name='YOUMEServiceProtocol.GetTipsRsp.tips_list', index=3,
      number=4, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
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
  serialized_start=385,
  serialized_end=495,
)

_APPTIPS.fields_by_name['comment'].message_type = comm_dot_youme__comm__pb2._MSGCOMMENT
_GETTIPSRSP.fields_by_name['tips_list'].message_type = _APPTIPS
DESCRIPTOR.message_types_by_name['AppTips'] = _APPTIPS
DESCRIPTOR.message_types_by_name['GetTipsRsp'] = _GETTIPSRSP

AppTips = _reflection.GeneratedProtocolMessageType('AppTips', (_message.Message,), dict(
  DESCRIPTOR = _APPTIPS,
  __module__ = 'tips_svr.youme_tips_pb2'
  # @@protoc_insertion_point(class_scope:YOUMEServiceProtocol.AppTips)
  ))
_sym_db.RegisterMessage(AppTips)

GetTipsRsp = _reflection.GeneratedProtocolMessageType('GetTipsRsp', (_message.Message,), dict(
  DESCRIPTOR = _GETTIPSRSP,
  __module__ = 'tips_svr.youme_tips_pb2'
  # @@protoc_insertion_point(class_scope:YOUMEServiceProtocol.GetTipsRsp)
  ))
_sym_db.RegisterMessage(GetTipsRsp)


DESCRIPTOR.has_options = True
DESCRIPTOR._options = _descriptor._ParseOptions(descriptor_pb2.FileOptions(), _b('H\003'))
# @@protoc_insertion_point(module_scope)