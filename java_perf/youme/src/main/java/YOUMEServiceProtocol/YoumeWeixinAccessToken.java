// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: weixin_access_token/youme_weixin_access_token.proto

package YOUMEServiceProtocol;

public final class YoumeWeixinAccessToken {
  private YoumeWeixinAccessToken() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }
  public interface GetWeiXinAccessTokenReqOrBuilder extends
      // @@protoc_insertion_point(interface_extends:YOUMEServiceProtocol.GetWeiXinAccessTokenReq)
      com.google.protobuf.MessageLiteOrBuilder {

    /**
     * <code>required int32 version = 1;</code>
     */
    boolean hasVersion();
    /**
     * <code>required int32 version = 1;</code>
     */
    int getVersion();
  }
  /**
   * <pre>
   * 获取微信凭证请求包
   * </pre>
   *
   * Protobuf type {@code YOUMEServiceProtocol.GetWeiXinAccessTokenReq}
   */
  public  static final class GetWeiXinAccessTokenReq extends
      com.google.protobuf.GeneratedMessageLite<
          GetWeiXinAccessTokenReq, GetWeiXinAccessTokenReq.Builder> implements
      // @@protoc_insertion_point(message_implements:YOUMEServiceProtocol.GetWeiXinAccessTokenReq)
      GetWeiXinAccessTokenReqOrBuilder {
    private GetWeiXinAccessTokenReq() {
    }
    private int bitField0_;
    public static final int VERSION_FIELD_NUMBER = 1;
    private int version_;
    /**
     * <code>required int32 version = 1;</code>
     */
    public boolean hasVersion() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    /**
     * <code>required int32 version = 1;</code>
     */
    public int getVersion() {
      return version_;
    }
    /**
     * <code>required int32 version = 1;</code>
     */
    private void setVersion(int value) {
      bitField0_ |= 0x00000001;
      version_ = value;
    }
    /**
     * <code>required int32 version = 1;</code>
     */
    private void clearVersion() {
      bitField0_ = (bitField0_ & ~0x00000001);
      version_ = 0;
    }

    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        output.writeInt32(1, version_);
      }
      unknownFields.writeTo(output);
    }

    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;

      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(1, version_);
      }
      size += unknownFields.getSerializedSize();
      memoizedSerializedSize = size;
      return size;
    }

    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, data);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, data, extensionRegistry);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, data);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, data, extensionRegistry);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, input);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, input, extensionRegistry);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return parseDelimitedFrom(DEFAULT_INSTANCE, input);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return parseDelimitedFrom(DEFAULT_INSTANCE, input, extensionRegistry);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, input);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, input, extensionRegistry);
    }

    public static Builder newBuilder() {
      return DEFAULT_INSTANCE.toBuilder();
    }
    public static Builder newBuilder(YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq prototype) {
      return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
    }

    /**
     * <pre>
     * 获取微信凭证请求包
     * </pre>
     *
     * Protobuf type {@code YOUMEServiceProtocol.GetWeiXinAccessTokenReq}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageLite.Builder<
          YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq, Builder> implements
        // @@protoc_insertion_point(builder_implements:YOUMEServiceProtocol.GetWeiXinAccessTokenReq)
        YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReqOrBuilder {
      // Construct using YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq.newBuilder()
      private Builder() {
        super(DEFAULT_INSTANCE);
      }


      /**
       * <code>required int32 version = 1;</code>
       */
      public boolean hasVersion() {
        return instance.hasVersion();
      }
      /**
       * <code>required int32 version = 1;</code>
       */
      public int getVersion() {
        return instance.getVersion();
      }
      /**
       * <code>required int32 version = 1;</code>
       */
      public Builder setVersion(int value) {
        copyOnWrite();
        instance.setVersion(value);
        return this;
      }
      /**
       * <code>required int32 version = 1;</code>
       */
      public Builder clearVersion() {
        copyOnWrite();
        instance.clearVersion();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:YOUMEServiceProtocol.GetWeiXinAccessTokenReq)
    }
    private byte memoizedIsInitialized = -1;
    protected final Object dynamicMethod(
        com.google.protobuf.GeneratedMessageLite.MethodToInvoke method,
        Object arg0, Object arg1) {
      switch (method) {
        case NEW_MUTABLE_INSTANCE: {
          return new YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq();
        }
        case IS_INITIALIZED: {
          byte isInitialized = memoizedIsInitialized;
          if (isInitialized == 1) return DEFAULT_INSTANCE;
          if (isInitialized == 0) return null;

          boolean shouldMemoize = ((Boolean) arg0).booleanValue();
          if (!hasVersion()) {
            if (shouldMemoize) {
              memoizedIsInitialized = 0;
            }
            return null;
          }
          if (shouldMemoize) memoizedIsInitialized = 1;
          return DEFAULT_INSTANCE;

        }
        case MAKE_IMMUTABLE: {
          return null;
        }
        case NEW_BUILDER: {
          return new Builder();
        }
        case VISIT: {
          Visitor visitor = (Visitor) arg0;
          YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq other = (YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq) arg1;
          version_ = visitor.visitInt(
              hasVersion(), version_,
              other.hasVersion(), other.version_);
          if (visitor == com.google.protobuf.GeneratedMessageLite.MergeFromVisitor
              .INSTANCE) {
            bitField0_ |= other.bitField0_;
          }
          return this;
        }
        case MERGE_FROM_STREAM: {
          com.google.protobuf.CodedInputStream input =
              (com.google.protobuf.CodedInputStream) arg0;
          com.google.protobuf.ExtensionRegistryLite extensionRegistry =
              (com.google.protobuf.ExtensionRegistryLite) arg1;
          try {
            boolean done = false;
            while (!done) {
              int tag = input.readTag();
              switch (tag) {
                case 0:
                  done = true;
                  break;
                default: {
                  if (!parseUnknownField(tag, input)) {
                    done = true;
                  }
                  break;
                }
                case 8: {
                  bitField0_ |= 0x00000001;
                  version_ = input.readInt32();
                  break;
                }
              }
            }
          } catch (com.google.protobuf.InvalidProtocolBufferException e) {
            throw new RuntimeException(e.setUnfinishedMessage(this));
          } catch (java.io.IOException e) {
            throw new RuntimeException(
                new com.google.protobuf.InvalidProtocolBufferException(
                    e.getMessage()).setUnfinishedMessage(this));
          } finally {
          }
        }
        case GET_DEFAULT_INSTANCE: {
          return DEFAULT_INSTANCE;
        }
        case GET_PARSER: {
          if (PARSER == null) {    synchronized (YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq.class) {
              if (PARSER == null) {
                PARSER = new DefaultInstanceBasedParser(DEFAULT_INSTANCE);
              }
            }
          }
          return PARSER;
        }
      }
      throw new UnsupportedOperationException();
    }


    // @@protoc_insertion_point(class_scope:YOUMEServiceProtocol.GetWeiXinAccessTokenReq)
    private static final YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq DEFAULT_INSTANCE;
    static {
      DEFAULT_INSTANCE = new GetWeiXinAccessTokenReq();
      DEFAULT_INSTANCE.makeImmutable();
    }

    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenReq getDefaultInstance() {
      return DEFAULT_INSTANCE;
    }

    private static volatile com.google.protobuf.Parser<GetWeiXinAccessTokenReq> PARSER;

    public static com.google.protobuf.Parser<GetWeiXinAccessTokenReq> parser() {
      return DEFAULT_INSTANCE.getParserForType();
    }
  }

  public interface GetWeiXinAccessTokenRspOrBuilder extends
      // @@protoc_insertion_point(interface_extends:YOUMEServiceProtocol.GetWeiXinAccessTokenRsp)
      com.google.protobuf.MessageLiteOrBuilder {

    /**
     * <code>required int32 version = 1;</code>
     */
    boolean hasVersion();
    /**
     * <code>required int32 version = 1;</code>
     */
    int getVersion();

    /**
     * <pre>
     * 返回码, 0成功, 其他失败
     * </pre>
     *
     * <code>required int32 ret = 2;</code>
     */
    boolean hasRet();
    /**
     * <pre>
     * 返回码, 0成功, 其他失败
     * </pre>
     *
     * <code>required int32 ret = 2;</code>
     */
    int getRet();

    /**
     * <code>required string token = 3;</code>
     */
    boolean hasToken();
    /**
     * <code>required string token = 3;</code>
     */
    java.lang.String getToken();
    /**
     * <code>required string token = 3;</code>
     */
    com.google.protobuf.ByteString
        getTokenBytes();

    /**
     * <code>required uint64 token_seq = 4;</code>
     */
    boolean hasTokenSeq();
    /**
     * <code>required uint64 token_seq = 4;</code>
     */
    long getTokenSeq();
  }
  /**
   * <pre>
   * 获取微信凭证应答包
   * </pre>
   *
   * Protobuf type {@code YOUMEServiceProtocol.GetWeiXinAccessTokenRsp}
   */
  public  static final class GetWeiXinAccessTokenRsp extends
      com.google.protobuf.GeneratedMessageLite<
          GetWeiXinAccessTokenRsp, GetWeiXinAccessTokenRsp.Builder> implements
      // @@protoc_insertion_point(message_implements:YOUMEServiceProtocol.GetWeiXinAccessTokenRsp)
      GetWeiXinAccessTokenRspOrBuilder {
    private GetWeiXinAccessTokenRsp() {
      token_ = "";
    }
    private int bitField0_;
    public static final int VERSION_FIELD_NUMBER = 1;
    private int version_;
    /**
     * <code>required int32 version = 1;</code>
     */
    public boolean hasVersion() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    /**
     * <code>required int32 version = 1;</code>
     */
    public int getVersion() {
      return version_;
    }
    /**
     * <code>required int32 version = 1;</code>
     */
    private void setVersion(int value) {
      bitField0_ |= 0x00000001;
      version_ = value;
    }
    /**
     * <code>required int32 version = 1;</code>
     */
    private void clearVersion() {
      bitField0_ = (bitField0_ & ~0x00000001);
      version_ = 0;
    }

    public static final int RET_FIELD_NUMBER = 2;
    private int ret_;
    /**
     * <pre>
     * 返回码, 0成功, 其他失败
     * </pre>
     *
     * <code>required int32 ret = 2;</code>
     */
    public boolean hasRet() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    /**
     * <pre>
     * 返回码, 0成功, 其他失败
     * </pre>
     *
     * <code>required int32 ret = 2;</code>
     */
    public int getRet() {
      return ret_;
    }
    /**
     * <pre>
     * 返回码, 0成功, 其他失败
     * </pre>
     *
     * <code>required int32 ret = 2;</code>
     */
    private void setRet(int value) {
      bitField0_ |= 0x00000002;
      ret_ = value;
    }
    /**
     * <pre>
     * 返回码, 0成功, 其他失败
     * </pre>
     *
     * <code>required int32 ret = 2;</code>
     */
    private void clearRet() {
      bitField0_ = (bitField0_ & ~0x00000002);
      ret_ = 0;
    }

    public static final int TOKEN_FIELD_NUMBER = 3;
    private java.lang.String token_;
    /**
     * <code>required string token = 3;</code>
     */
    public boolean hasToken() {
      return ((bitField0_ & 0x00000004) == 0x00000004);
    }
    /**
     * <code>required string token = 3;</code>
     */
    public java.lang.String getToken() {
      return token_;
    }
    /**
     * <code>required string token = 3;</code>
     */
    public com.google.protobuf.ByteString
        getTokenBytes() {
      return com.google.protobuf.ByteString.copyFromUtf8(token_);
    }
    /**
     * <code>required string token = 3;</code>
     */
    private void setToken(
        java.lang.String value) {
      if (value == null) {
    throw new NullPointerException();
  }
  bitField0_ |= 0x00000004;
      token_ = value;
    }
    /**
     * <code>required string token = 3;</code>
     */
    private void clearToken() {
      bitField0_ = (bitField0_ & ~0x00000004);
      token_ = getDefaultInstance().getToken();
    }
    /**
     * <code>required string token = 3;</code>
     */
    private void setTokenBytes(
        com.google.protobuf.ByteString value) {
      if (value == null) {
    throw new NullPointerException();
  }
  bitField0_ |= 0x00000004;
      token_ = value.toStringUtf8();
    }

    public static final int TOKEN_SEQ_FIELD_NUMBER = 4;
    private long tokenSeq_;
    /**
     * <code>required uint64 token_seq = 4;</code>
     */
    public boolean hasTokenSeq() {
      return ((bitField0_ & 0x00000008) == 0x00000008);
    }
    /**
     * <code>required uint64 token_seq = 4;</code>
     */
    public long getTokenSeq() {
      return tokenSeq_;
    }
    /**
     * <code>required uint64 token_seq = 4;</code>
     */
    private void setTokenSeq(long value) {
      bitField0_ |= 0x00000008;
      tokenSeq_ = value;
    }
    /**
     * <code>required uint64 token_seq = 4;</code>
     */
    private void clearTokenSeq() {
      bitField0_ = (bitField0_ & ~0x00000008);
      tokenSeq_ = 0L;
    }

    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        output.writeInt32(1, version_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        output.writeInt32(2, ret_);
      }
      if (((bitField0_ & 0x00000004) == 0x00000004)) {
        output.writeString(3, getToken());
      }
      if (((bitField0_ & 0x00000008) == 0x00000008)) {
        output.writeUInt64(4, tokenSeq_);
      }
      unknownFields.writeTo(output);
    }

    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;

      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(1, version_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(2, ret_);
      }
      if (((bitField0_ & 0x00000004) == 0x00000004)) {
        size += com.google.protobuf.CodedOutputStream
          .computeStringSize(3, getToken());
      }
      if (((bitField0_ & 0x00000008) == 0x00000008)) {
        size += com.google.protobuf.CodedOutputStream
          .computeUInt64Size(4, tokenSeq_);
      }
      size += unknownFields.getSerializedSize();
      memoizedSerializedSize = size;
      return size;
    }

    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, data);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, data, extensionRegistry);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, data);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, data, extensionRegistry);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, input);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, input, extensionRegistry);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return parseDelimitedFrom(DEFAULT_INSTANCE, input);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return parseDelimitedFrom(DEFAULT_INSTANCE, input, extensionRegistry);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, input);
    }
    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageLite.parseFrom(
          DEFAULT_INSTANCE, input, extensionRegistry);
    }

    public static Builder newBuilder() {
      return DEFAULT_INSTANCE.toBuilder();
    }
    public static Builder newBuilder(YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp prototype) {
      return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
    }

    /**
     * <pre>
     * 获取微信凭证应答包
     * </pre>
     *
     * Protobuf type {@code YOUMEServiceProtocol.GetWeiXinAccessTokenRsp}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageLite.Builder<
          YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp, Builder> implements
        // @@protoc_insertion_point(builder_implements:YOUMEServiceProtocol.GetWeiXinAccessTokenRsp)
        YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRspOrBuilder {
      // Construct using YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp.newBuilder()
      private Builder() {
        super(DEFAULT_INSTANCE);
      }


      /**
       * <code>required int32 version = 1;</code>
       */
      public boolean hasVersion() {
        return instance.hasVersion();
      }
      /**
       * <code>required int32 version = 1;</code>
       */
      public int getVersion() {
        return instance.getVersion();
      }
      /**
       * <code>required int32 version = 1;</code>
       */
      public Builder setVersion(int value) {
        copyOnWrite();
        instance.setVersion(value);
        return this;
      }
      /**
       * <code>required int32 version = 1;</code>
       */
      public Builder clearVersion() {
        copyOnWrite();
        instance.clearVersion();
        return this;
      }

      /**
       * <pre>
       * 返回码, 0成功, 其他失败
       * </pre>
       *
       * <code>required int32 ret = 2;</code>
       */
      public boolean hasRet() {
        return instance.hasRet();
      }
      /**
       * <pre>
       * 返回码, 0成功, 其他失败
       * </pre>
       *
       * <code>required int32 ret = 2;</code>
       */
      public int getRet() {
        return instance.getRet();
      }
      /**
       * <pre>
       * 返回码, 0成功, 其他失败
       * </pre>
       *
       * <code>required int32 ret = 2;</code>
       */
      public Builder setRet(int value) {
        copyOnWrite();
        instance.setRet(value);
        return this;
      }
      /**
       * <pre>
       * 返回码, 0成功, 其他失败
       * </pre>
       *
       * <code>required int32 ret = 2;</code>
       */
      public Builder clearRet() {
        copyOnWrite();
        instance.clearRet();
        return this;
      }

      /**
       * <code>required string token = 3;</code>
       */
      public boolean hasToken() {
        return instance.hasToken();
      }
      /**
       * <code>required string token = 3;</code>
       */
      public java.lang.String getToken() {
        return instance.getToken();
      }
      /**
       * <code>required string token = 3;</code>
       */
      public com.google.protobuf.ByteString
          getTokenBytes() {
        return instance.getTokenBytes();
      }
      /**
       * <code>required string token = 3;</code>
       */
      public Builder setToken(
          java.lang.String value) {
        copyOnWrite();
        instance.setToken(value);
        return this;
      }
      /**
       * <code>required string token = 3;</code>
       */
      public Builder clearToken() {
        copyOnWrite();
        instance.clearToken();
        return this;
      }
      /**
       * <code>required string token = 3;</code>
       */
      public Builder setTokenBytes(
          com.google.protobuf.ByteString value) {
        copyOnWrite();
        instance.setTokenBytes(value);
        return this;
      }

      /**
       * <code>required uint64 token_seq = 4;</code>
       */
      public boolean hasTokenSeq() {
        return instance.hasTokenSeq();
      }
      /**
       * <code>required uint64 token_seq = 4;</code>
       */
      public long getTokenSeq() {
        return instance.getTokenSeq();
      }
      /**
       * <code>required uint64 token_seq = 4;</code>
       */
      public Builder setTokenSeq(long value) {
        copyOnWrite();
        instance.setTokenSeq(value);
        return this;
      }
      /**
       * <code>required uint64 token_seq = 4;</code>
       */
      public Builder clearTokenSeq() {
        copyOnWrite();
        instance.clearTokenSeq();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:YOUMEServiceProtocol.GetWeiXinAccessTokenRsp)
    }
    private byte memoizedIsInitialized = -1;
    protected final Object dynamicMethod(
        com.google.protobuf.GeneratedMessageLite.MethodToInvoke method,
        Object arg0, Object arg1) {
      switch (method) {
        case NEW_MUTABLE_INSTANCE: {
          return new YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp();
        }
        case IS_INITIALIZED: {
          byte isInitialized = memoizedIsInitialized;
          if (isInitialized == 1) return DEFAULT_INSTANCE;
          if (isInitialized == 0) return null;

          boolean shouldMemoize = ((Boolean) arg0).booleanValue();
          if (!hasVersion()) {
            if (shouldMemoize) {
              memoizedIsInitialized = 0;
            }
            return null;
          }
          if (!hasRet()) {
            if (shouldMemoize) {
              memoizedIsInitialized = 0;
            }
            return null;
          }
          if (!hasToken()) {
            if (shouldMemoize) {
              memoizedIsInitialized = 0;
            }
            return null;
          }
          if (!hasTokenSeq()) {
            if (shouldMemoize) {
              memoizedIsInitialized = 0;
            }
            return null;
          }
          if (shouldMemoize) memoizedIsInitialized = 1;
          return DEFAULT_INSTANCE;

        }
        case MAKE_IMMUTABLE: {
          return null;
        }
        case NEW_BUILDER: {
          return new Builder();
        }
        case VISIT: {
          Visitor visitor = (Visitor) arg0;
          YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp other = (YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp) arg1;
          version_ = visitor.visitInt(
              hasVersion(), version_,
              other.hasVersion(), other.version_);
          ret_ = visitor.visitInt(
              hasRet(), ret_,
              other.hasRet(), other.ret_);
          token_ = visitor.visitString(
              hasToken(), token_,
              other.hasToken(), other.token_);
          tokenSeq_ = visitor.visitLong(
              hasTokenSeq(), tokenSeq_,
              other.hasTokenSeq(), other.tokenSeq_);
          if (visitor == com.google.protobuf.GeneratedMessageLite.MergeFromVisitor
              .INSTANCE) {
            bitField0_ |= other.bitField0_;
          }
          return this;
        }
        case MERGE_FROM_STREAM: {
          com.google.protobuf.CodedInputStream input =
              (com.google.protobuf.CodedInputStream) arg0;
          com.google.protobuf.ExtensionRegistryLite extensionRegistry =
              (com.google.protobuf.ExtensionRegistryLite) arg1;
          try {
            boolean done = false;
            while (!done) {
              int tag = input.readTag();
              switch (tag) {
                case 0:
                  done = true;
                  break;
                default: {
                  if (!parseUnknownField(tag, input)) {
                    done = true;
                  }
                  break;
                }
                case 8: {
                  bitField0_ |= 0x00000001;
                  version_ = input.readInt32();
                  break;
                }
                case 16: {
                  bitField0_ |= 0x00000002;
                  ret_ = input.readInt32();
                  break;
                }
                case 26: {
                  String s = input.readString();
                  bitField0_ |= 0x00000004;
                  token_ = s;
                  break;
                }
                case 32: {
                  bitField0_ |= 0x00000008;
                  tokenSeq_ = input.readUInt64();
                  break;
                }
              }
            }
          } catch (com.google.protobuf.InvalidProtocolBufferException e) {
            throw new RuntimeException(e.setUnfinishedMessage(this));
          } catch (java.io.IOException e) {
            throw new RuntimeException(
                new com.google.protobuf.InvalidProtocolBufferException(
                    e.getMessage()).setUnfinishedMessage(this));
          } finally {
          }
        }
        case GET_DEFAULT_INSTANCE: {
          return DEFAULT_INSTANCE;
        }
        case GET_PARSER: {
          if (PARSER == null) {    synchronized (YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp.class) {
              if (PARSER == null) {
                PARSER = new DefaultInstanceBasedParser(DEFAULT_INSTANCE);
              }
            }
          }
          return PARSER;
        }
      }
      throw new UnsupportedOperationException();
    }


    // @@protoc_insertion_point(class_scope:YOUMEServiceProtocol.GetWeiXinAccessTokenRsp)
    private static final YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp DEFAULT_INSTANCE;
    static {
      DEFAULT_INSTANCE = new GetWeiXinAccessTokenRsp();
      DEFAULT_INSTANCE.makeImmutable();
    }

    public static YOUMEServiceProtocol.YoumeWeixinAccessToken.GetWeiXinAccessTokenRsp getDefaultInstance() {
      return DEFAULT_INSTANCE;
    }

    private static volatile com.google.protobuf.Parser<GetWeiXinAccessTokenRsp> PARSER;

    public static com.google.protobuf.Parser<GetWeiXinAccessTokenRsp> parser() {
      return DEFAULT_INSTANCE.getParserForType();
    }
  }


  static {
  }

  // @@protoc_insertion_point(outer_class_scope)
}
