#ifndef XNA_GRAPHICS_EFFECT_HPP
#define XNA_GRAPHICS_EFFECT_HPP

#include "../common/numerics.hpp"
#include "../default.hpp"
#include "gresource.hpp"

namespace xna {   
    //Represents an annotation to an EffectParameter. 
    class EffectAnnotation {
    public:        
        Int ColumCount() const;
        String Name() const;
        EffectParameterClass ParameterClass() const;
        Int Rowcount() const;
        String Semantic() const;
        bool GetValueBoolean() const;
        Int GetValueInt32() const;
        Matrix GetValueMatrix() const;
        float GetValueSingle() const;
        String GetValueString() const;
        Vector2 GetValueVector2() const;
        Vector3 GetValueVector3() const;
        Vector4 GetValueVector4() const;

    public:
        struct PlatformImplementation;
        uptr<PlatformImplementation> impl;

    public:
        EffectAnnotation();
    };

    using PEffectAnnotation = sptr<EffectAnnotation>;

    class EffectAnnotationCollection {
    public:
        EffectAnnotationCollection();
        
        EffectAnnotationCollection(std::vector<PEffectAnnotation> const& data) : data(data)
        {
        }

        constexpr size_t Count() const {
            return data.size();
        }

        PEffectAnnotation operator[](size_t index) {
            if (index >= data.size())
                return nullptr;

            return data[index];
        }

        PEffectAnnotation operator[](String const& name) {
            for (size_t i = 0; i < data.size(); ++i) {
                const auto& p = data[i];

                if (p->Name() == name)
                    return p;
            }

            return nullptr;
        }

    public:
        std::vector<PEffectAnnotation> data;
    };

    using PEffectAnnotationCollection = sptr<EffectAnnotationCollection>;

    class EffectPass {
    public:        
        //Gets the name of this pass. 
        String Name() const;
        //The EffectAnnotationCollection containing EffectAnnotation objects for this EffectPass.
        PEffectAnnotationCollection Annotations() const;

        //Begins this pass.
        void Apply();
    public:
        struct PlatformImplementation;
        uptr<PlatformImplementation> impl;    

    public:
        EffectPass(sptr<GraphicsDevice> const& device);
    };

	class Effect : public GraphicsResource {
        Effect(sptr<GraphicsDevice> const& device, std::vector<Byte> const& effectCode);

    public:
        struct PlatformImplementation;
        uptr<PlatformImplementation> impl;
    };

    class IEffectMatrices {
        virtual Matrix World() const = 0;
        virtual Matrix View() const = 0;
        virtual Matrix Projection() const = 0;

        virtual void World(Matrix const& value) = 0;
        virtual void View(Matrix const& value) = 0;
        virtual void Projection(Matrix const& value) = 0;
    };

    class DirectionalLight;

    class IEffectLights {
        virtual DirectionalLight DirectionalLight0() const = 0;
        virtual DirectionalLight DirectionalLight1() const = 0;
        virtual DirectionalLight DirectionalLight2() const = 0;

        virtual Vector3 AmbientLightColor() const = 0;
        virtual void AmbientLightColor(Vector3 const& value) = 0;

        virtual bool LightingEnabled() const = 0;
        virtual void LightingEnabled(bool value) = 0;

        virtual void EnableDefaultLighting() = 0;
    };

    class IEffectFog
    {
        virtual bool FogEnabled() const = 0;
        virtual float FogStart() const = 0;
        virtual float FogEnd() const = 0;
        virtual Vector3 FogColor() const = 0;

        virtual void FogEnabled(bool value) const = 0;
        virtual void FogStart(float value) const = 0;
        virtual void FogEnd(float value) const = 0;
        virtual void FogColor(Vector3 const& value) const = 0;
    };
}

#endif