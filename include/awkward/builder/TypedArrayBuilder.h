// BSD 3-Clause License; see https://github.com/scikit-hep/awkward-1.0/blob/main/LICENSE

#ifndef AWKWARD_TYPEDARRAYBUILDER_H_
#define AWKWARD_TYPEDARRAYBUILDER_H_

#include "awkward/common.h"
#include "awkward/util.h"

namespace awkward {
  class Content;
  using ContentPtr = std::shared_ptr<Content>;

  class Form;
  using FormPtr = std::shared_ptr<Form>;

  class FormBuilder;
  using FormBuilderPtr = std::shared_ptr<FormBuilder>;

  using DataPtr = std::shared_ptr<void>;

  class BitMaskedForm;
  using BitMaskedFormPtr = std::shared_ptr<BitMaskedForm>;

  class ByteMaskedForm;
  using ByteMaskedFormPtr = std::shared_ptr<ByteMaskedForm>;

  class EmptyForm;
  using EmptyFormPtr = std::shared_ptr<EmptyForm>;

  class IndexedForm;
  using IndexedFormPtr = std::shared_ptr<IndexedForm>;

  class IndexedOptionForm;
  using IndexedOptionFormPtr = std::shared_ptr<IndexedOptionForm>;

  class ListForm;
  using ListFormPtr = std::shared_ptr<ListForm>;

  class ListOffsetForm;
  using ListOffsetFormPtr = std::shared_ptr<ListOffsetForm>;

  class NumpyForm;
  using NumpyFormPtr = std::shared_ptr<NumpyForm>;

  class RawForm;
  using RawFormPtr = std::shared_ptr<RawForm>;

  class RecordForm;
  using RecordFormPtr = std::shared_ptr<RecordForm>;

  class RegularForm;
  using RegularFormPtr = std::shared_ptr<RegularForm>;

  class UnionForm;
  using UnionFormPtr = std::shared_ptr<UnionForm>;

  class UnmaskedForm;
  using UnmaskedFormPtr = std::shared_ptr<UnmaskedForm>;

  class VirtualForm;
  using VirtualFormPtr = std::shared_ptr<VirtualForm>;

  struct Data {
    Data(const DataPtr& iptr, const int64_t ilength)
      : ptr(iptr),
        length(ilength) {}

    const DataPtr ptr;
    int64_t length;
  };

  struct Sum
  {
    void operator()(const std::unique_ptr<Data>& data) { length += data->length; }
    int64_t length {0};
  };

  /// @class FormBuilder
  ///
  /// @brief Abstract base class for nodes within a TypedArrayBuilder
  class LIBAWKWARD_EXPORT_SYMBOL FormBuilder {
  public:
    /// @brief Virtual destructor acts as a first non-inline virtual function
    /// that determines a specific translation unit in which vtable shall be
    /// emitted.
    virtual ~FormBuilder();

    /// @brief User-friendly name of this class.
    virtual const std::string
      classname() const = 0;

    /// @brief Turns the accumulated data into a Content array.
    virtual const ContentPtr
      snapshot() const = 0;

    /// @brief (FIXME: ?)
    virtual void
      set_input_buffer(const DataPtr& data) = 0;

    /// @brief (FIXME: ?)
    virtual void
      set_data_length(int64_t length) = 0;

    /// @brief (FIXME: ?)
    virtual int64_t
      length() const { return 0; }

    /// @brief Add a Form to interpret the accumulated data.
    ///
    /// Creates a nested FormBuilder if the given Form is accepted by this
    /// builder Form.
    virtual int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) = 0;

    /// @brief
    virtual bool
      accepts(awkward::util::dtype dt) const { return false; }

    /// @brief
    virtual void
      integer(int64_t x) {
        throw std::runtime_error(
          std::string("FIXME: 'integer' is not implemented "));
    }

    /// @brief
    virtual void
      boolean(bool x) {
        throw std::runtime_error(
          std::string("FIXME: 'boolean' is not implemented "));
    }

    /// @brief
    virtual void
      real(double x) {
        throw std::runtime_error(
          std::string("FIXME: 'real' is not implemented "));
    }

    /// @brief
    virtual const FormBuilderPtr&
      field_check(const char* key) const {
        throw std::invalid_argument(
          std::string("FIXME: 'field_check' is not implemented "));
    }

  };

  /// @class BitMaskedArrayBuilder
  ///
  /// @brief BitMaskedArray builder from a BitMaskedForm
  class LIBAWKWARD_EXPORT_SYMBOL BitMaskedArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a BitMaskedArrayBuilder from a full set of parameters.
    BitMaskedArrayBuilder(const BitMaskedFormPtr& form,
                          const DataPtr& data,
                          int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief Creates a 'content' FormBuilder if the 'form' is accepted by this
    /// builder Form.
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    /// @brief BitMaskedForm that defines the BitMaskedArray.
    const BitMaskedFormPtr form_;

    /// @brief Accumulated data is an IndexU8 mask.
    DataPtr data_;

    /// @brief Number of elements in the mask array.
    int64_t length_;

    /// @brief Content
    FormBuilderPtr content_;
  };

  /// @class ByteMaskedArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL ByteMaskedArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a ByteMaskedArrayBuilder from a full set of parameters.
    ByteMaskedArrayBuilder(const ByteMaskedFormPtr& form,
                           const DataPtr& data,
                           int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief Creates a 'content' FormBuilder if the 'form' is accepted by this
    /// builder Form.
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const ByteMaskedFormPtr form_;
    DataPtr data_;
    int64_t length_;
    FormBuilderPtr content_;
  };

  /// @class EmptyArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL EmptyArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates an EmptyArrayBuilder from a full set of parameters.
    EmptyArrayBuilder(const EmptyFormPtr& form,
                      const DataPtr& data,
                      int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief An EmptyForm does not accept other Forms.
    /// It throws an 'invalid_argument' exception.
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const EmptyFormPtr form_;
    DataPtr data_;
    int64_t length_;
  };

  /// @class IndexedArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL IndexedArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates an IndexedArrayBuilder from a full set of parameters.
    IndexedArrayBuilder(const IndexedFormPtr& form,
                        const DataPtr& data,
                        int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief Creates a 'content' FormBuilder if the 'form' is accepted by this
    /// builder Form.
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const IndexedFormPtr form_;
    DataPtr data_;
    int64_t length_;
    FormBuilderPtr content_;
  };

  /// @class IndexedOptionArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL IndexedOptionArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates an IndexedOptionArrayBuilder from a full set of parameters.
    IndexedOptionArrayBuilder(const IndexedOptionFormPtr& form,
                              const DataPtr& data,
                              int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief Creates a 'content' FormBuilder if the 'form' is accepted by this
    /// builder Form.
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const IndexedOptionFormPtr form_;
    DataPtr data_;
    int64_t length_;
    FormBuilderPtr content_;
  };

  /// @class ListArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL ListArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a ListArrayBuilder from a full set of parameters.
    ListArrayBuilder(const ListFormPtr& form,
                     const DataPtr& data,
                     int64_t length,
                     bool copyarrays = true);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief Creates a 'content' FormBuilder if the 'form' is accepted by this
    /// builder Form.
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const ListFormPtr form_;
    DataPtr data_;
    int64_t length_;
    FormBuilderPtr content_;
    bool copyarrays_;
  };

  /// @class ListOffsetArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL ListOffsetArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a ListOffsetArrayBuilder from a full set of parameters.
    ListOffsetArrayBuilder(const ListOffsetFormPtr& form,
                           const DataPtr& data,
                           int64_t length,
                           bool copyarrays = true);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief Creates a 'content' FormBuilder if the 'form' is accepted by this
    /// builder Form.
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const ListOffsetFormPtr form_;
    DataPtr data_;
    int64_t length_;
    FormBuilderPtr content_;
    bool copyarrays_;
  };

  /// @class NumpyArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL NumpyArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a NumpyArrayBuilder from a full set of parameters.
    NumpyArrayBuilder(const NumpyFormPtr& form,
                      const DataPtr& data,
                      int64_t length,
                      bool copyarrays = true);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        // FIXME:
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        // FIXME:
    }

    /// @brief (FIXME: ?)
    int64_t
      length() const override;

    /// @brief A NumpyForm does not accept other Forms.
    /// An 'invalid_argument' exception is thrown.
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

    /// @brief Checks if the dtype is accepted by the Form.
    bool
      accepts(awkward::util::dtype dt) const override;

    /// @brief
    void
      integer(int64_t x) override;

    /// @brief
    void
      boolean(bool x) override;

    /// @brief
    void
      real(double x) override;

  private:
    const NumpyFormPtr form_;
    std::vector<std::unique_ptr<Data>> data_;
    int64_t length_;
    bool copyarrays_;
  };

  /// @class RawArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL RawArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a RawArrayBuilder from a full set of parameters.
    RawArrayBuilder(const EmptyFormPtr& form,
                    const DataPtr& data,
                    int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const RawFormPtr form_;
    DataPtr data_;
    int64_t length_;
  };

  /// @class RecordArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL RecordArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a RecordArrayBuilder from a full set of parameters.
    RecordArrayBuilder(const RecordFormPtr& form,
                       const DataPtr& data,
                       int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief Creates a 'content' FormBuilder if the 'form' is accepted by this
    /// builder Form.
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

    const FormBuilderPtr&
      field_check(const char* key) const override;

  private:
    const RecordFormPtr form_;
    DataPtr data_;
    int64_t length_;
    std::vector<FormBuilderPtr> contents_;
    std::vector<std::string> keys_;
  };

  /// @class RegularArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL RegularArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a RegularArrayBuilder from a full set of parameters.
    RegularArrayBuilder(const RegularFormPtr& form,
                        const DataPtr& data,
                        int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const RegularFormPtr form_;
    DataPtr data_;
    int64_t length_;
    FormBuilderPtr content_;
  };

  /// @class UnionArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL UnionArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a UnionArrayBuilder from a full set of parameters.
    UnionArrayBuilder(const UnionFormPtr& form,
                      const DataPtr& data,
                      int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    /// @brief (FIXME: ?)
    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const UnionFormPtr form_;
    DataPtr data_;
    int64_t length_;
    std::vector<FormBuilderPtr> contents_;
  };

  /// @class UnmaskedArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL UnmaskedArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates an UnmaskedArrayBuilder from a full set of parameters.
    UnmaskedArrayBuilder(const UnmaskedFormPtr& form,
                         const DataPtr& data,
                         int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const UnmaskedFormPtr form_;
    DataPtr data_;
    int64_t length_;
  };

  /// @class VirtualArrayBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL VirtualArrayBuilder : public FormBuilder {
  public:
    /// @brief Creates a VirtualArrayBuilder from a full set of parameters.
    VirtualArrayBuilder(const VirtualFormPtr& form,
                        const DataPtr& data,
                        int64_t length);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot() const override;

    /// @brief (FIXME: ?)
    void
      set_input_buffer(const DataPtr& data) override {
        data_ = data;
    }

    /// @brief (FIXME: ?)
    void
      set_data_length(int64_t length) override {
        length_ = length;
    }

    int64_t
      apply(const FormPtr& form, const DataPtr& data, const int64_t length) override;

  private:
    const VirtualFormPtr form_;
    DataPtr data_;
    int64_t length_;
  };

  /// @class TypedArrayBuilder
  ///
  /// @brief User interface to the FormBuilder system: the TypedArrayBuilder is a
  /// fixed reference while the FormBuilder subclass instances change in
  /// response to accumulating data.
  class LIBAWKWARD_EXPORT_SYMBOL TypedArrayBuilder {
  public:
    /// @brief Creates an TypedArrayBuilder from a full set of parameters.
    ///
    /// @param initial The initial number of entries for a buffer.
    TypedArrayBuilder(int64_t initial, const DataPtr& data = nullptr, const int64_t length = 0);

    /// @brief Add a Form to interpret the accumulated data.
    ///
    /// Creates a Root node of the FormBuilder if it has not been defined,
    /// e.g. a 'nullptr', otherwise passes it to the FormBuilder.
    void
      apply(const FormPtr& form, const DataPtr& data = nullptr, const int64_t length = 0);

    /// @brief Turns the accumulated data into a Content array.
    ///
    /// This operation only converts FormBuilder nodes into Content nodes; the
    /// buffers holding array data are shared between the FormBuilder and the
    /// Content. Hence, taking a snapshot is a constant-time operation.
    const ContentPtr
      snapshot() const;

    /// @brief Sets an Input buffer (FIXME: growable?)
    void
      set_input_buffer(const DataPtr& data);

    /// @brief Sets an Input buffer length (FIXME: when it's done?)
    void
      set_data_length(const int64_t length);

    /// @brief Access to the internal buffer (FIXME: ?)
    const DataPtr&
      data_buffer() const {
        return data_;
      }

    /// @brief Length of the internal buffer (FIXME: ?)
    int64_t
      length() const {
        return length_;
      }

    /// @brief
    void
      field_check(const char* key);

    /// @brief
    void
      integer(int64_t x);

    /// @brief
    void
      boolean(bool x);

    /// @brief
    void
      real(double x);

  private:
    /// See #initial.
    int64_t initial_;

    /// @brief Root node of the FormBuilder tree.
    std::shared_ptr<FormBuilder> builder_;

    /// @brief Current node of the FormBuilder tree.
    std::shared_ptr<FormBuilder> current_builder_;

    /// @brief Pointer to an Input buffer. (FIXME: ?)
    DataPtr data_;

    /// @brief Length of a Content array. (FIXME: ?)
    int64_t length_;
  };

}

#endif // AWKWARD_TYPEDARRAYBUILDER_H_