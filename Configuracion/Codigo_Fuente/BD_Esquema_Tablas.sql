-- 1. Creación de la base de datos
CREATE DATABASE "RosasMonteserin";

-- 2. Creación del esquema para organizar los objetos relacionados con sensores
CREATE SCHEMA "Sensores";
-- 3. Creación y configuración de usuario con privilegios

-- Crear usuario con contraseña
CREATE USER lfgelo WITH PASSWORD '12345';

-- Conceder permisos básicos sobre el esquema
GRANT USAGE, CREATE ON SCHEMA "Sensores" TO lfgelo;
GRANT TEMPORARY ON DATABASE "RosasMonteserin" TO lfgelo;

-- Conceder todos los privilegios sobre tablas y secuencias existentes
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA "Sensores" TO lfgelo;
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA "Sensores" TO lfgelo;

-- Configurar privilegios por defecto para futuros objetos
ALTER DEFAULT PRIVILEGES IN SCHEMA "Sensores"
GRANT ALL PRIVILEGES ON TABLES TO lfgelo;

ALTER DEFAULT PRIVILEGES IN SCHEMA "Sensores"
GRANT ALL PRIVILEGES ON SEQUENCES TO lfgelo;

-- 4. Creación de tablas

-- Tabla catálogo de sensores (almacena información general de cada tipo de sensor)
CREATE TABLE "Sensores".FCAT_SENSOR (
  CAT_IDSENSOR     SERIAL PRIMARY KEY,                     -- ID autoincremental (clave primaria)
  CAT_DESCRIPCION  TEXT NOT NULL UNIQUE,                  -- Descripción única del sensor
  CAT_TIPO         TEXT NOT NULL,                          -- Tipo de sensor (temperatura, humedad, etc.)
  CAT_UNIMEDIDA    TEXT NOT NULL,                          -- Unidad de medida (ºC, %, etc.)
  CAT_ESTADO       CHAR(1) NOT NULL DEFAULT '1',           -- Estado (1=activo, 0=inactivo)
  CAT_FECHCREA     TIMESTAMPTZ NOT NULL DEFAULT NOW()      -- Fecha creación con zona horaria
);

-- Tabla de lecturas de sensores (registra las mediciones de cada sensor)
CREATE TABLE "Sensores".FLEC_SENSOR (
  LEC_IDLECTURA     SERIAL PRIMARY KEY,                    -- ID autoincremental (clave primaria)
  LEC_CATIDESENS    INTEGER NOT NULL REFERENCES "Sensores".FCAT_SENSOR (CAT_IDSENSOR) 
                     ON DELETE CASCADE ON UPDATE CASCADE,  -- Relación con tabla catálogo (eliminación/actualización en cascada)
  LEC_LECTURA       DOUBLE PRECISION NOT NULL,              -- Valor numérico de la lectura
  LEC_FECHLECTURA   TIMESTAMPTZ NOT NULL DEFAULT NOW()     -- Fecha lectura con zona horaria
);

