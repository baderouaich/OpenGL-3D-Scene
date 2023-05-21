//
// Created by bader on 5/21/23.
//

#pragma once
#include <string>
#include <memory>
#include <map>
#include <Model/Model.hpp>
#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <filesystem>
namespace fs = std::filesystem;

class ResourceManager {
private:
    inline static std::map<std::string, std::shared_ptr<Model>> m_models;
    inline static std::map<std::string, std::shared_ptr<Shader>> m_shaders;
    inline static std::map<std::string, std::shared_ptr<Texture>> m_textures;

public:
    static void LoadTexture(const fs::path& filename, Texture::TextureType type, const std::string& key){
      if(HasTexture(key)) {
        std::cerr << "Texture " << key << " already loaded\n";
        return;
      }
      std::shared_ptr<Texture> tex(new Texture(filename, type));
      m_textures[key] = std::move(tex);
    }
    static const std::shared_ptr<Texture>& GetTexture(const std::string& key){
      if(not HasTexture(key)) {
        throw std::runtime_error("Texture " + key + " doesn't exist");
      }
      return m_textures[key];
    }
    static bool HasTexture(const std::string& key){
      return m_textures.find(key) != m_textures.end();
    }



public:
    static void Clear() {
      m_models.clear();
      m_shaders.clear();
      m_textures.clear();
    }
};

