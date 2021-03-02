// BSD 3-Clause License; see https://github.com/scikit-hep/awkward-1.0/blob/main/LICENSE

#ifndef AWKWARD_UNKNOWNFORMBUILDER_H_
#define AWKWARD_UNKNOWNFORMBUILDER_H_

#include "awkward/typedbuilder/FormBuilder.h"

namespace awkward {

  /// @class UnknownFormBuilder
  ///
  /// @brief
  class LIBAWKWARD_EXPORT_SYMBOL UnknownFormBuilder : public FormBuilder {
  public:
    /// @brief Creates a VirtualArrayBuilder from a full set of parameters.
    UnknownFormBuilder(const FormPtr& form);

    /// @brief User-friendly name of this class.
    const std::string
      classname() const override;

    /// @brief Turns the accumulated data into a Content array.
    const ContentPtr
      snapshot(const ForthOtputBufferMap& outputs) const override;

    /// @brief
    const FormPtr
      form() const override;

    /// @brief
    const std::string
      vm_output() const override;

    /// @brief
    const std::string
      vm_func() const override;

    /// @brief
    const std::string
      vm_func_name() const override;

    private:
      const FormPtr form_;
      std::string vm_empty_command_;
  };

}

#endif // AWKWARD_UNKNOWNFORMBUILDER_H_