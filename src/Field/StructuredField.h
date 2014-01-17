#ifndef __Geomtk_StructuredField__
#define __Geomtk_StructuredField__

#include "Field.h"
#include "StructuredMesh.h"

namespace geomtk {

/**
 *  This class specifies the scalar field on structured mesh. The data type is
 *  templated, so any proper basic type (e.g. double) and classes can be used.
 */
template <typename T>
class StructuredField : public Field {
protected:
    field<TimeLevels<field<T>, 2>*> data;
    field<vector<StaggerType> > staggerTypes;
    FieldType fieldType;
    ArakawaGrid gridType;
public:
    StructuredField(const Mesh &mesh, bool hasHalfLevel = false);
    StructuredField(const string &name, const string &units,
                    const string &longName, const Mesh &mesh,
                    bool hasHalfLevel = false);
    virtual ~StructuredField();

    /**
     *  Create the memory storage for the scalar field.
     *
     *  @param fieldType    the field type.
     *  @param xStaggerType the stagger type of x grids.
     *  @param yStaggerType the stagger type of y grids.
     */
    void create(FieldType fieldType, StaggerType xStaggerType,
                StaggerType yStaggerType);

    /**
     *  Create the memory storage for the scalar field.
     *
     *  @param fieldType    the field type.
     *  @param xStaggerType the stagger type of x grids.
     *  @param yStaggerType the stagger type of y grids.
     *  @param zStaggerType the stagger type of z grids.
     */
    void create(FieldType fieldType, StaggerType xStaggerType,
                StaggerType yStaggerType, StaggerType zStaggerType);

    /**
     *  Create the memory storage for the vector field.
     *
     *  @param fieldType     the field type.
     *  @param xStaggerType0 the stagger type of x grids of the 1st component.
     *  @param yStaggerType0 the stagger type of y grids of the 1st component.
     *  @param xStaggerType1 the stagger type of x grids of the 2nd component.
     *  @param yStaggerType1 the stagger type of y grids of the 2nd component.
     */
    void create(FieldType fieldType, StaggerType xStaggerType0,
                StaggerType yStaggerType0, StaggerType xStaggerType1,
                StaggerType yStaggerType1);

    /**
     *  Create the memory storage for the vector field.
     *
     *  @param fieldType     the field type.
     *  @param xStaggerType0 the stagger type of x grids of the 1st component.
     *  @param yStaggerType0 the stagger type of y grids of the 1st component.
     *  @param zStaggerType0 the stagger type of z grids of the 1st component.
     *  @param xStaggerType1 the stagger type of x grids of the 2nd component.
     *  @param yStaggerType1 the stagger type of y grids of the 2nd component.
     *  @param zStaggerType0 the stagger type of z grids of the 2nd component.
     *  @param xStaggerType2 the stagger type of x grids of the 3rd component.
     *  @param yStaggerType2 the stagger type of y grids of the 3rd component.
     *  @param zStaggerType0 the stagger type of z grids of the 3rd component.
     */
    void create(FieldType fieldType, StaggerType xStaggerType0,
                StaggerType yStaggerType0, StaggerType zStaggerType0,
                StaggerType xStaggerType1, StaggerType yStaggerType1,
                StaggerType zStaggerType1, StaggerType xStaggerType2,
                StaggerType yStaggerType2, StaggerType zStaggerType2);

    /**
     *  Create the memory storage for the field.
     *
     *  @param fieldType the field type.
     *  @param numDim    the dimension number.
     *  @param gridType  the grid type (Arakawa grid type).
     */
    void create(FieldType fieldType, int numDim, ArakawaGrid gridType);

    /**
     *  Apply boundary conditions after the scalar field is updated.
     *
     *  @param timeIdx       the time level index.
     *  @param updateHalfLevel the flag for updating half level.
     */
    template <typename = typename enable_if<has_operator_plus<T>::value ||
                                            is_arithmetic<T>::value>::type>
    void applyBndCond(const TimeLevelIndex<2> &timeIdx,
                      bool updateHalfLevel = false) {
        for (int m = 0; m < data.size(); ++m) {
            int nx = data(m)->getLevel(0).n_rows;
            int ny = data(m)->getLevel(0).n_cols;
            int nz = data(m)->getLevel(0).n_slices;
            if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
                for (int k = 0; k < nz; ++k) {
                    for (int j = 0; j < ny; ++j) {
                        data(m)->getLevel(timeIdx)(0, j, k) =
                        data(m)->getLevel(timeIdx)(nx-2, j, k);
                        data(m)->getLevel(timeIdx)(nx-1, j, k) =
                        data(m)->getLevel(timeIdx)(1, j, k);
                    }
                }
            } else {
                REPORT_ERROR("Under construction!");
            }
            if (updateHalfLevel && data(m)->hasHalfLevel()) {
                TimeLevelIndex<2> halfTimeIdx = timeIdx-0.5;
                TimeLevelIndex<2> oldTimeIdx = timeIdx-1;
                for (int m = 0; m < data.size(); ++m) {
                    int nx = data(m)->getLevel(0).n_rows;
                    int ny = data(m)->getLevel(0).n_cols;
                    int nz = data(m)->getLevel(0).n_slices;
                    for (int k = 0; k < nz; ++k) {
                        for (int j = 0; j < ny; ++j) {
                            for (int i = 0; i < nx; ++i) {
                                data(m)->getLevel(halfTimeIdx)(i, j, k) =
                                (data(m)->getLevel(oldTimeIdx)(i, j, k)+
                                 data(m)->getLevel(timeIdx)(i, j, k))*0.5;
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     *  Subscript operator of the scalar field.
     *
     *  @param timeIdx the time level index.
     *  @param i       the index of x dimension.
     *  @param j       the index of y dimension.
     *  @param k       the index of z dimension.
     *
     *  @return The scalar on the given indices.
     */
    T operator()(const TimeLevelIndex<2> &timeIdx, int i, int j) const;
    T& operator()(const TimeLevelIndex<2> &timeIdx, int i, int j);
    T operator()(const TimeLevelIndex<2> &timeIdx, int i, int j, int k) const;
    T& operator()(const TimeLevelIndex<2> &timeIdx, int i, int j, int k);
    T operator()(int l, int i, int j) const;
    T& operator()(int l, int i, int j);
    T operator()(int l, int i, int j, int k) const;
    T& operator()(int l, int i, int j, int k);
    
    /**
     *  Subscript operator of the vector field.
     *
     *  @param comp    the component index.
     *  @param timeIdx the time level index.
     *  @param i       the index of x dimension.
     *  @param j       the index of y dimension.
     *  @param k       the index of z dimension.
     *
     *  @return The vector component on the given indices.
     */
    T operator()(int comp, const TimeLevelIndex<2> &timeIdx, int i, int j) const;
    T& operator()(int comp, const TimeLevelIndex<2> &timeIdx, int i, int j);
    T operator()(int comp, const TimeLevelIndex<2> &timeIdx, int i, int j, int k) const;
    T& operator()(int comp, const TimeLevelIndex<2> &timeIdx, int i, int j, int k);

    /**
     *  Get the stagger type on the given dimension of the scalar field.
     *
     *  @param dim the dimension index.
     *
     *  @return The stagger type on the given dimension.
     */
    inline StaggerType getStaggerType(int dim) const;
    
    /**
     *  Get the stagger type of the given component on the given dimension of
     *  the vector field.
     *
     *  @param comp the component index.
     *  @param dim  the dimension index.
     *
     *  @return The stagger type of the given component on the given dimension.
     */
    inline StaggerType getStaggerType(int comp, int dim) const;

    /**
     *  Convert the 2D vector field onto the given grid type.
     *
     *  @param gridType the grid type (Arakawa grid type).
     *  @param timeIdx  the time level index.
     *  @param xc        the x component.
     *  @param yc        the y component.
     */
    void convert(ArakawaGrid gridType, const TimeLevelIndex<2> &timeIdx,
                 mat &xc, mat &yc);

    /**
     *  Convert the 3D vector field onto the given grid type.
     *
     *  @param gridType the grid type (Arakawa grid type).
     *  @param timeIdx  the time level index.
     *  @param xc       the x component.
     *  @param yc       the y component.
     *  @param zc       the z component.
     */
    void convert(ArakawaGrid gridType, const TimeLevelIndex<2> &timeIdx,
                 cube &xc, cube &yc, cube &zc);
};

// -----------------------------------------------------------------------------

template <typename T>
StructuredField<T>::StructuredField(const Mesh &mesh, bool hasHalfLevel)
: Field(mesh, hasHalfLevel) {
    if (dynamic_cast<const StructuredMesh*>(&mesh) == NULL) {
        REPORT_ERROR("Mesh should comply with StructuredMesh!");
    }
}

template <typename T>
StructuredField<T>::StructuredField(const string &name, const string &units,
                                    const string &longName, const Mesh &mesh,
                                    bool hasHalfLevel)
: Field(name, units, longName, mesh, hasHalfLevel) {
    if (dynamic_cast<const StructuredMesh*>(&mesh) == NULL) {
        REPORT_ERROR("Mesh should comply with StructuredMesh!");
    }
}

template <typename T>
StructuredField<T>::~StructuredField() {
    for (int i = 0; i < data.size(); ++i) {
        delete data(i);
    }
}

template <typename T>
void StructuredField<T>::create(FieldType fieldType, StaggerType xStaggerType,
                                StaggerType yStaggerType) {
    if (fieldType != ScalarField) {
        REPORT_ERROR("Field should be a scalar!");
    }
    this->fieldType = fieldType;
    staggerTypes.set_size(1);
    staggerTypes(0).resize(2);
    staggerTypes(0)[0] = xStaggerType;
    staggerTypes(0)[1] = yStaggerType;
    const StructuredMesh &mesh = static_cast<const StructuredMesh&>(*(this->mesh));
    data.set_size(1);
    data(0) = new TimeLevels<field<T>, 2>(hasHalfLevel);
    for (int i = 0; i < data[0]->getNumLevel(INCLUDE_HALF_LEVEL); ++i) {
        data(0)->getLevel(i).set_size(mesh.getNumGrid(0, xStaggerType, true),
                                      mesh.getNumGrid(1, yStaggerType));
    }
}

template <typename T>
void StructuredField<T>::create(FieldType fieldType, StaggerType xStaggerType,
                                StaggerType yStaggerType, StaggerType zStaggerType) {
    if (fieldType != ScalarField) {
        REPORT_ERROR("Field should be a scalar!")
    }
    this->fieldType = fieldType;
    staggerTypes.set_size(1);
    staggerTypes(0).resize(3);
    staggerTypes(0)[0] = xStaggerType;
    staggerTypes(0)[1] = yStaggerType;
    staggerTypes(0)[2] = zStaggerType;
    const StructuredMesh &mesh = static_cast<const StructuredMesh&>(*(this->mesh));
    data.set_size(1);
    data(0) = new TimeLevels<field<T>, 2>(hasHalfLevel);
    for (int i = 0; i < data[0]->getNumLevel(INCLUDE_HALF_LEVEL); ++i) {
        data(0)->getLevel(i).set_size(mesh.getNumGrid(0, xStaggerType, true),
                                      mesh.getNumGrid(1, yStaggerType),
                                      mesh.getNumGrid(2, zStaggerType));
    }
}

template <typename T>
void StructuredField<T>::create(FieldType fieldType, StaggerType xStaggerType0,
                                StaggerType yStaggerType0, StaggerType xStaggerType1,
                                StaggerType yStaggerType1) {
    if (fieldType != VectorField) {
        REPORT_ERROR("Field should be a vector!");
    }
    this->fieldType = fieldType;
    staggerTypes.set_size(2);
    staggerTypes(0).resize(2);
    staggerTypes(1).resize(2);
    staggerTypes(0)[0] = xStaggerType0;
    staggerTypes(0)[1] = yStaggerType0;
    staggerTypes(1)[0] = xStaggerType1;
    staggerTypes(1)[1] = yStaggerType1;
    const StructuredMesh &mesh = static_cast<const StructuredMesh&>(*(this->mesh));
    data.set_size(2);
    for (int m = 0; m < data.size(); ++m) {
        data(m) = new TimeLevels<field<T>, 2>(hasHalfLevel);
        for (int i = 0; i < data[0]->getNumLevel(INCLUDE_HALF_LEVEL); ++i) {
            data(m)->getLevel(i).set_size(mesh.getNumGrid(0, staggerTypes[m][0], true),
                                          mesh.getNumGrid(1, staggerTypes[m][1]));
        }
    }
}

template <typename T>
void StructuredField<T>::create(FieldType fieldType, StaggerType xStaggerType0,
                                StaggerType yStaggerType0, StaggerType zStaggerType0,
                                StaggerType xStaggerType1, StaggerType yStaggerType1,
                                StaggerType zStaggerType1, StaggerType xStaggerType2,
                                StaggerType yStaggerType2, StaggerType zStaggerType2) {
    if (fieldType != VectorField) {
        REPORT_ERROR("Field should be a vector!");
    }
    this->fieldType = fieldType;
    staggerTypes.set_size(3);
    staggerTypes(0).resize(3);
    staggerTypes(1).resize(3);
    staggerTypes(2).resize(3);
    staggerTypes(0)[0] = xStaggerType0;
    staggerTypes(0)[1] = yStaggerType0;
    staggerTypes(0)[2] = zStaggerType0;
    staggerTypes(1)[0] = xStaggerType1;
    staggerTypes(1)[1] = yStaggerType1;
    staggerTypes(1)[2] = zStaggerType1;
    staggerTypes(2)[0] = xStaggerType2;
    staggerTypes(2)[1] = yStaggerType2;
    staggerTypes(2)[2] = zStaggerType2;
    const StructuredMesh &mesh = static_cast<const StructuredMesh&>(*(this->mesh));
    data.set_size(3);
    for (int m = 0; m < data.size(); ++m) {
        data(m) = new TimeLevels<field<T>, 2>(hasHalfLevel);
        for (int i = 0; i < data[0]->getNumLevel(INCLUDE_HALF_LEVEL); ++i) {
            data(m)->getLevel(i).set_size(mesh.getNumGrid(0, staggerTypes[m][0], true),
                                          mesh.getNumGrid(1, staggerTypes[m][1]),
                                          mesh.getNumGrid(2, staggerTypes[m][2]));
        }
    }
}

template <typename T>
void StructuredField<T>::create(FieldType fieldType, int numDim,
                                ArakawaGrid gridType) {
    if (fieldType == ScalarField && gridType != A_GRID) {
        REPORT_ERROR("Scalar field should be on A grids!");
    }
    this->gridType = gridType;
    switch (gridType) {
        case A_GRID:
            if (numDim == 2) {
                if (fieldType == ScalarField) {
                    create(fieldType, CENTER, CENTER);
                } else if (fieldType == VectorField) {
                    create(fieldType, CENTER, CENTER, CENTER, CENTER);
                }
            } else if (numDim == 3) {
                if (fieldType == ScalarField) {
                    create(fieldType, CENTER, CENTER, CENTER);
                } else if (fieldType == VectorField) {
                    create(fieldType, CENTER, CENTER, CENTER,
                                      CENTER, CENTER, CENTER,
                                      CENTER, CENTER, CENTER);
                }
            }
            break;
        case B_GRID:
            REPORT_ERROR("Under construction!");
            break;
        case C_GRID:
            if (numDim == 2) {
                create(fieldType,   EDGE, CENTER,
                                  CENTER,   EDGE);
            } else if (numDim == 3) {
                create(fieldType,   EDGE, CENTER, CENTER,
                                  CENTER,   EDGE, CENTER,
                                  CENTER, CENTER,   EDGE);
            }
            break;
        case D_GRID:
            REPORT_ERROR("Under construction!");
            break;
        case E_GRID:
            REPORT_ERROR("Under construction!");
            break;
    }
}

template <typename T>
T StructuredField<T>::operator()(const TimeLevelIndex<2> &timeIdx,
                                 int i, int j) const {
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(0)->getLevel(timeIdx)(I, J);
}

template <typename T>
T& StructuredField<T>::operator()(const TimeLevelIndex<2> &timeIdx,
                                  int i, int j) {
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(0)->getLevel(timeIdx)(I, J);
}

template <typename T>
T StructuredField<T>::operator()(const TimeLevelIndex<2> &timeIdx,
                                 int i, int j, int k) const {
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(0)->getLevel(timeIdx)(I, J, k);
}

template <typename T>
T& StructuredField<T>::operator()(const TimeLevelIndex<2> &timeIdx,
                                  int i, int j, int k) {
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(0)->getLevel(timeIdx)(I, J, k);
}

template <typename T>
T StructuredField<T>::operator()(int l, int i, int j) const {
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(0)->getLevel(l)(I, J);
}

template <typename T>
T& StructuredField<T>::operator()(int l, int i, int j) {
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(0)->getLevel(l)(I, J);
}

template <typename T>
T StructuredField<T>::operator()(int l, int i, int j, int k) const {
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(0)->getLevel(l)(I, J, k);
}

template <typename T>
T& StructuredField<T>::operator()(int l, int i, int j, int k) {
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(0)->getLevel(l)(I, J, k);
}
    
template <typename T>
T StructuredField<T>::operator()(int comp, const TimeLevelIndex<2> &timeIdx,
                                 int i, int j) const {
#ifdef DEBUG
    if (fieldType != VectorField) {
        REPORT_ERROR("Field should be a vector!");
    }
#endif
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(comp)->getLevel(timeIdx)(I, J);
}

template <typename T>
T& StructuredField<T>::operator()(int comp, const TimeLevelIndex<2> &timeIdx,
                                  int i, int j) {
#ifdef DEBUG
    if (fieldType != VectorField) {
        REPORT_ERROR("Field should be a vector!");
    }
#endif
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(comp)->getLevel(timeIdx)(I, J);
}

template <typename T>
T StructuredField<T>::operator()(int comp, const TimeLevelIndex<2> &timeIdx,
                                 int i, int j, int k) const {
#ifdef DEBUG
    if (fieldType != VectorField) {
        REPORT_ERROR("Field should be a vector!");
    }
#endif
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(comp)->getLevel(timeIdx)(I, J, k);
}

template <typename T>
T& StructuredField<T>::operator()(int comp, const TimeLevelIndex<2> &timeIdx,
                                  int i, int j, int k) {
#ifdef DEBUG
    if (fieldType != VectorField) {
        REPORT_ERROR("Field should be a vector!");
    }
#endif
    // The virtual boundary grids at the periodic boundary conditions are hiden
    // from user.
    int I, J;
    if (mesh->getDomain().getAxisStartBndType(0) == PERIODIC) {
        I = i+1;
    } else {
        I = i;
    }
    if (mesh->getDomain().getAxisStartBndType(1) == PERIODIC) {
        J = j+1;
    } else {
        J = j;
    }
    return data(comp)->getLevel(timeIdx)(I, J, k);
}

template <typename T>
StaggerType StructuredField<T>::getStaggerType(int dim) const {
    if (fieldType != ScalarField) {
        REPORT_ERROR("Field is not a scalar!");
    }
    return staggerTypes(0)[dim];
}

template <typename T>
StaggerType StructuredField<T>::getStaggerType(int comp, int dim) const {
    if (fieldType != VectorField) {
        REPORT_ERROR("Field is not a vector!");
    }
    return staggerTypes(comp)[dim];
}

template <typename T>
void StructuredField<T>::convert(ArakawaGrid gridType,
                                 const TimeLevelIndex<2> &timeIdx,
                                 mat &xc, mat &yc) {
    if (fieldType != VectorField) {
        REPORT_ERROR("Field is not a vector!");
    }
    if (data.size() != 2) {
        REPORT_ERROR("Field is not 2D!");
    }
    const StructuredMesh &mesh = static_cast<const StructuredMesh&>(*(this->mesh));
    if (this->gridType == C_GRID && gridType == A_GRID) {
        if (xc.n_rows != mesh.getNumGrid(0, CENTER)) {
            REPORT_ERROR("Argument 'xc' dimension does not match!");
        } else if (xc.n_cols != mesh.getNumGrid(1, CENTER)) {
            REPORT_ERROR("Argument 'xc' dimension does not match!");
        }
        if (yc.n_rows != mesh.getNumGrid(0, CENTER)) {
            REPORT_ERROR("Argument 'yc' dimension does not match!");
        } else if (yc.n_cols != mesh.getNumGrid(1, CENTER)) {
            REPORT_ERROR("Argument 'yc' dimension does not match!");
        }
        if (mesh.getDomain().getAxisStartBndType(0) == PERIODIC) {
            for (int j = 0; j < mesh.getNumGrid(1, CENTER); ++j) {
                for (int i = 0; i < mesh.getNumGrid(0, CENTER); ++i) {
                    xc(i, j) = ((*this)(0, timeIdx, i-1, j)+
                                (*this)(0, timeIdx,   i, j))*0.5;
                }
            }
        } else {
            REPORT_ERROR("Under construction!");
        }
        if (mesh.getDomain().getAxisStartBndType(1) == POLE) {
            for (int j = 1; j < mesh.getNumGrid(1, CENTER)-1; ++j) {
                for (int i = 0; i < mesh.getNumGrid(0, CENTER); ++i) {
                    yc(i, j) = ((*this)(1, timeIdx, i,   j)+
                                (*this)(1, timeIdx, i, j-1))*0.5;
                }
            }
        } else {
            REPORT_ERROR("Under construction!");
        }
    }
}

template <typename T>
void StructuredField<T>::convert(ArakawaGrid gridType,
                                 const TimeLevelIndex<2> &timeIdx,
                                 cube &xc, cube &yc, cube &zc) {
    const StructuredMesh &mesh = static_cast<const StructuredMesh&>(*(this->mesh));
    if (fieldType != VectorField) {
        REPORT_ERROR("Field is not a vector!");
    }
    if (data.size() != 3) {
        REPORT_ERROR("Field is not 3D!");
    }
    if (this->gridType == C_GRID && gridType == A_GRID) {
        if (xc.n_rows != mesh.getNumGrid(0, CENTER)) {
            REPORT_ERROR("Argument 'xc' dimension does not match!");
        } else if (xc.n_cols != mesh.getNumGrid(1, CENTER)) {
            REPORT_ERROR("Argument 'xc' dimension does not match!");
        } else if (xc.n_slices != mesh.getNumGrid(2, CENTER)) {
            REPORT_ERROR("Argument 'xc' dimension does not match!");
        }
        if (yc.n_rows != mesh.getNumGrid(0, CENTER)) {
            REPORT_ERROR("Argument 'yc' dimension does not match!");
        } else if (yc.n_cols != mesh.getNumGrid(1, CENTER)) {
            REPORT_ERROR("Argument 'yc' dimension does not match!");
        } else if (yc.n_slices != mesh.getNumGrid(2, CENTER)) {
            REPORT_ERROR("Argument 'yc' dimension does not match!");
        }
        if (zc.n_rows != mesh.getNumGrid(0, CENTER)) {
            REPORT_ERROR("Argument 'zc' dimension does not match!");
        } else if (zc.n_cols != mesh.getNumGrid(1, CENTER)) {
            REPORT_ERROR("Argument 'zc' dimension does not match!");
        } else if (zc.n_slices != mesh.getNumGrid(2, CENTER)) {
            REPORT_ERROR("Argument 'zc' dimension does not match!");
        }
        if (mesh.getDomain().getAxisStartBndType(0) == PERIODIC) {
            for (int k = 0; k < mesh.getNumGrid(2, CENTER); ++k) {
                for (int j = 0; j < mesh.getNumGrid(1, CENTER); ++j) {
                    for (int i = 0; i < mesh.getNumGrid(0, CENTER); ++i) {
                        xc(i, j, k) = (data[0]->getLevel(timeIdx)(  i, j, k)+
                                       data[0]->getLevel(timeIdx)(i+1, j, k))*0.5;
                    }
                }
            }
        } else {
            REPORT_ERROR("Under construction!");
        }
        if (mesh.getDomain().getAxisStartBndType(1) == POLE) {
            for (int k = 0; k < mesh.getNumGrid(2, CENTER); ++k) {
                for (int j = 1; j < mesh.getNumGrid(1, CENTER)-1; ++j) {
                    for (int i = 0; i < mesh.getNumGrid(0, CENTER); ++i) {
                        yc(i, j, k) = (data[1]->getLevel(timeIdx)(i,   j, k)+
                                       data[1]->getLevel(timeIdx)(i, j-1, k))*0.5;
                    }
                }
            }
        } else {
            REPORT_ERROR("Under construction!");
        }
        if (mesh.getDomain().getAxisStartBndType(2) == RIGID) {
            for (int k = 1; k < mesh.getNumGrid(2, CENTER)-1; ++k) {
                for (int j = 0; j < mesh.getNumGrid(1, CENTER); ++j) {
                    for (int i = 0; i < mesh.getNumGrid(0, CENTER); ++i) {
                        zc(i, j, k) = (data[2]->getLevel(timeIdx)(i, j, k)+
                                       data[2]->getLevel(timeIdx)(i, j, k-1))*0.5;
                    }
                }
            }
        } else {
            REPORT_ERROR("Under construction!");
        }
    }
}
    
}

#endif