/*
 * terrainmodel.h
 * Copyright 2008-2009, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2009, Edward Hutchins <eah1@yahoo.com>
 * Copyright 2012, Manu Evans <turkeyman@gmail.com>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TERRAINMODEL_H
#define TERRAINMODEL_H

#include <QAbstractItemModel>
#include <tileset.h>

namespace Tiled {

class Tileset;
class Terrain;

namespace Internal {

class MapDocument;

/**
 * A model providing a tree view on the terrain types available on a map.
 */
class TerrainModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param tileset the initial tileset to display
     */
    TerrainModel(MapDocument *mapDocument,
                 QObject *parent = 0);

    ~TerrainModel();

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(Tileset *tileset) const;
    QModelIndex index(Terrain *terrain, int column = 0) const;

    QModelIndex parent(const QModelIndex &child) const;

    /**
     * Returns the number of rows. For the root, this is the number of tilesets
     * with terrain types defined. Otherwise it is the number of terrain types
     * in a certain tileset.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * Returns the number of columns.
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * Returns the data stored under the given <i>role</i> for the item
     * referred to by the <i>index</i>.
     */
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const;

    /**
     * Allows for changing the name of a terrain.
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    /**
     * Makes terrain names are editable.
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
     * Returns a small size hint, to prevent the headers from affecting the
     * minimum width and height of the sections.
     */
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    /**
     * Returns the tileset at the given index, or 0 if there is no tileset.
     */
    Tileset *tilesetAt(const QModelIndex &index) const;

    /**
     * Returns the terrain at the given index, or 0 if there is no terrain.
     */
    Terrain *terrainAt(const QModelIndex &index) const;

    void insertTerrain(Tileset *tileset, int index, Terrain *terrain);
    Terrain *takeTerrainAt(Tileset *tileset, int index);
    void setTerrainName(Tileset *tileset, int index, const QString &name);
    void setTerrainImage(Tileset *tileset, int index, int tileId);

signals:
    void terrainAdded(Tileset *tileset, int terrainId);
    void terrainRemoved(Tileset *tileset, int terrainId);

    /**
     * Emitted when either the name or the image of a terrain changed.
     */
    void terrainChanged(Tileset *tileset, int index);

private:
    // TODO: Get rid of this class. Should be possible when:
    //  index.internalPointer is only set for terrains
    //  when it is not set, it points to tilesetAt(row)
    //  when it is set, it points to tileset->terrainAt(row)
    struct Node
    {
        enum NodeType {
            TilesetNode,
            TerrainNode
        };

        Node(Tileset *tileset) : type(TilesetNode), tileset(tileset) {}
        Node(Terrain *terrain) : type(TerrainNode), terrain(terrain) {}

        bool isTileset() const { return type == TilesetNode; }
        bool isTerrain() const { return type == TerrainNode; }

        NodeType type;
        union {
            Tileset *tileset;
            Terrain *terrain;
        };
    };

    void emitTerrainChanged(Terrain *terrain);

    MapDocument *mMapDocument;
    QHash<Tileset*, Node*> mTilesetNodes;
    QHash<Terrain*, Node*> mTerrainNodes;
};

} // namespace Internal
} // namespace Tiled

#endif // TERRAINMODEL_H
