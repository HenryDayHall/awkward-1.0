// BSD 3-Clause License; see https://github.com/scikit-hep/awkward-1.0/blob/main/LICENSE

#ifndef AWKWARD_TYPEDARRAYBUILDER_H_
#define AWKWARD_TYPEDARRAYBUILDER_H_

#include "awkward/common.h"
#include "awkward/util.h"
#include "awkward/forth/ForthMachine.h"
#include "awkward/typedbuilder/FormBuilder.h"

#include <complex>

namespace awkward {
  class ArrayBuilderOptions;

  class Content;
  using ContentPtr = std::shared_ptr<Content>;
  class Slice;
  class Type;
  using TypePtr = std::shared_ptr<Type>;

  class Form;
  using FormPtr = std::shared_ptr<Form>;

  using ForthOtputBufferMap = std::map<std::string, std::shared_ptr<ForthOutputBuffer>>;

  class FormBuilder;
  using FormBuilderPtr = std::shared_ptr<FormBuilder>;

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
    TypedArrayBuilder(const FormPtr& form, const ArrayBuilderOptions& options);

    /// @brief
    void
      connect(const std::shared_ptr<ForthMachine32>& vm);

    /// @brief
    void
      debug_step() const;

    /// @brief
    const FormPtr
      form() const;

    /// @brief
    const std::string
      to_vm() const;

    /// @brief Returns a string representation of this array (single-line XML
    /// indicating the length and type).
    const std::string
      tostring() const;

    /// @brief Current length of the accumulated array.
    int64_t
      length() const;

    /// @brief Removes all accumulated data without resetting the type
    /// knowledge.
    void
      clear();

    /// @brief Current high level Type of the accumulated array.
    ///
    /// @param typestrs A mapping from `"__record__"` parameters to string
    /// representations of those types, to override the derived strings.
    const TypePtr
      type(const util::TypeStrs& typestrs) const;

    /// @brief Turns the accumulated data into a Content array.
    ///
    /// This operation only converts FormBuilder nodes into Content nodes; the
    /// buffers holding array data are shared between the FormBuilder and the
    /// Content. Hence, taking a snapshot is a constant-time operation.
    const ContentPtr
      snapshot() const;

    /// @brief Returns the element at a given position in the array, handling
    /// negative indexing and bounds-checking like Python.
    ///
    /// The first item in the array is at `0`, the second at `1`, the last at
    /// `-1`, the penultimate at `-2`, etc.
    const ContentPtr
      getitem_at(int64_t at) const;

    /// @brief Subinterval of this array, handling negative indexing
    /// and bounds-checking like Python.
    ///
    /// The first item in the array is at `0`, the second at `1`, the last at
    /// `-1`, the penultimate at `-2`, etc.
    ///
    /// Ranges beyond the array are not an error; they are trimmed to
    /// `start = 0` on the left and `stop = length() - 1` on the right.
    const ContentPtr
      getitem_range(int64_t start, int64_t stop) const;

    /// @brief This array with the first nested RecordArray replaced by
    /// the field at `key`.
    const ContentPtr
      getitem_field(const std::string& key) const;

    /// @brief This array with the first nested RecordArray replaced by
    /// a RecordArray of a given subset of `keys`.
    const ContentPtr
      getitem_fields(const std::vector<std::string>& keys) const;

    /// @brief Entry point for general slicing: Slice represents a tuple of
    /// SliceItem nodes applying to each level of nested lists.
    const ContentPtr
      getitem(const Slice& where) const;

    /// @brief Adds a `null` value to the accumulated data.
    void
      null();

    /// @brief Adds a boolean value `x` to the accumulated data.
    void
      boolean(bool x);

    /// @brief Adds an integer value `x` to the accumulated data.
    void
      integer(int64_t x);

    /// @brief Adds a real value `x` to the accumulated data.
    void
      real(double x);

    /// @brief Adds a complex value `x` to the accumulated data.
    void
      complex(std::complex<double> x);

    /// @brief Adds an unencoded, null-terminated bytestring value `x` to the
    /// accumulated data.
    void
      bytestring(const char* x);

    /// @brief Adds an unencoded bytestring value `x` with a given `length`
    /// to the accumulated data.
    ///
    /// The string does not need to be null-terminated.
    void
      bytestring(const char* x, int64_t length);

    /// @brief Adds an unencoded bytestring `x` in STL format to the
    /// accumulated data.
    void
      bytestring(const std::string& x);

    /// @brief Adds a UTF-8 encoded, null-terminated bytestring value `x` to
    /// the accumulated data.
    void
      string(const char* x);

    /// @brief Adds a UTF-8 encoded bytestring value `x` with a given `length`
    /// to the accumulated data.
    ///
    /// The string does not need to be null-terminated.
    void
      string(const char* x, int64_t length);

    /// @brief Adds a UTF-8 encoded bytestring `x` in STL format to the
    /// accumulated data.
    void
      string(const std::string& x);

    /// @brief Begins building a nested list.
    ///
    /// The first 'beginlist' puts AwkwardForth VM into a state that expects
    /// another 'beginlist' or 'endlist'.
    /// The second puts the VM into a state that expects 'integer', etc.
    /// or 'endlist'.
    void
      beginlist();

    /// @brief Ends a nested list.
    void
      endlist();

    /// @brief Begins building a tuple with a fixed number of fields.
    void
      begintuple(int64_t numfields);

    /// @brief Sets the pointer to a given tuple field index; the next
    /// command will fill that slot.
    void
      index(int64_t index);

    /// @brief Ends a tuple.
    void
      endtuple();

    /// @brief Begins building a record without a name.
    ///
    /// See #beginrecord_fast and #beginrecord_check.
    void
      beginrecord();

    /// @brief Begins building a record with a name.
    ///
    /// @param name This name is used to distinguish
    /// records of different types in heterogeneous data (to build a
    /// union of record arrays, rather than a record array with union
    /// fields and optional values) and it also sets the `"__record__"`
    /// parameter to later add custom behaviors in Python.
    ///
    /// In the `_fast` version of this method, a string comparison is not
    /// performed: the same pointer is assumed to have the same value each time
    /// (safe for string literals).
    ///
    /// See #beginrecord and #beginrecord_check.
    void
      beginrecord_fast(const char* name);

    /// @brief Begins building a record with a name.
    ///
    /// @param name This name is used to distinguish
    /// records of different types in heterogeneous data (to build a
    /// union of record arrays, rather than a record array with union
    /// fields and optional values) and it also sets the `"__record__"`
    /// parameter to later add custom behaviors in Python.
    ///
    /// In the `_check` version of this method, a string comparison is
    /// performed every time it is called to verify that the `name` matches
    /// a stored `name`.
    ///
    /// See #beginrecord and #beginrecord_fast.
    void
      beginrecord_check(const char* name);

    /// @brief Begins building a record with a name.
    ///
    /// @param name This name is used to distinguish
    /// records of different types in heterogeneous data (to build a
    /// union of record arrays, rather than a record array with union
    /// fields and optional values) and it also sets the `"__record__"`
    /// parameter to later add custom behaviors in Python.
    ///
    /// In the `_check` version of this method, a string comparison is
    /// performed every time it is called to verify that the `name` matches
    /// a stored `name`.
    ///
    /// See #beginrecord and #beginrecord_fast.
    void
      beginrecord_check(const std::string& name);

    /// @brief Sets the pointer to a given record field `key`; the next
    /// command will fill that slot.
    ///
    /// In the `_fast` version of this method, a string comparison is not
    /// performed: the same pointer is assumed to have the same value each time
    /// (safe for string literals). See #field_check.
    ///
    /// Record keys are checked in round-robin order. The best performance
    /// will be achieved by filling them in the same order for each record.
    /// Lookup time for random order scales with the number of fields.
    void
      field_fast(const char* key);

    /// @brief Sets the pointer to a given record field `key`; the next
    /// command will fill that slot.
    ///
    /// In the `_check` version of this method, a string comparison is
    /// performed every time it is called to verify that the `key` matches
    /// a stored `key`. See #field_fast.
    ///
    /// Record keys are checked in round-robin order. The best performance
    /// will be achieved by filling them in the same order for each record.
    /// Lookup time for random order scales with the number of fields.
    void
      field_check(const char* key);

    /// @brief Sets the pointer to a given record field `key`; the next
    /// command will fill that slot.
    ///
    /// In the `_check` version of this method, a string comparison is
    /// performed every time it is called to verify that the `key` matches
    /// a stored `key`. See #field_fast.
    ///
    /// Record keys are checked in round-robin order. The best performance
    /// will be achieved by filling them in the same order for each record.
    /// Lookup time for random order scales with the number of fields.
    void
      field_check(const std::string& key);

    /// @brief Ends a record.
    void
      endrecord();

    /// @brief Append an element `at` a given index of an arbitrary `array`
    /// (Content instance) to the accumulated data, handling
    /// negative indexing and bounds-checking like Python.
    ///
    /// The first item in the array is at `0`, the second at `1`, the last at
    /// `-1`, the penultimate at `-2`, etc.
    ///
    /// The resulting #snapshot will be an {@link IndexedArrayOf IndexedArray}
    /// that shares data with the provided `array`.
    void
      append(const ContentPtr& array, int64_t at);

    /// @brief Append an element `at` a given index of an arbitrary `array`
    /// (Content instance) to the accumulated data, without
    /// handling negative indexing or bounds-checking.
    ///
    /// The resulting #snapshot will be an {@link IndexedArrayOf IndexedArray}
    /// that shares data with the provided `array`.
    void
      append_nowrap(const ContentPtr& array, int64_t at);

    /// @brief Extend the accumulated data with an entire `array`.
    ///
    /// The resulting #snapshot will be an {@link IndexedArrayOf IndexedArray}
    /// that shares data with the provided `array`.
    void
      extend(const ContentPtr& array);

  private:
    /// See #initial.
    int64_t initial_;
    int64_t length_;

    std::string vm_input_data_;
    std::string vm_source_;

    /// @brief Root node of the FormBuilder tree.
    std::shared_ptr<FormBuilder> builder_;

    /// @brief Current node of the FormBuilder tree.
    std::shared_ptr<FormBuilder> current_builder_;

    std::shared_ptr<ForthMachine32> vm_;
    std::map<std::string, std::shared_ptr<ForthInputBuffer>> vm_inputs_map_;
  };

}

#endif // AWKWARD_TYPEDARRAYBUILDER_H_