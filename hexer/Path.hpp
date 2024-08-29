/******************************************************************************
 * Copyright (c) 2014, Hobu Inc. (howard@hobu.co)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following
 * conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of the Howard Butler or Hobu, Inc.
 *       the names of its contributors may be
 *       used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 ****************************************************************************/
#pragma once

#include <vector>
#include <ostream>

#include "Mathpair.hpp"
#include "Segment.hpp"

namespace hexer
{

enum Orientation
{
    CLOCKWISE,     // Outer
    ANTICLOCKWISE  // Hole
};

class Path
{
public:
    Path(int pathNum) : m_pathNum(pathNum), m_parent(NULL)
    {}

    void add(const Segment& s)
        { m_segments.push_back(s); }
    void addChild(Path path)
        { m_children.push_back(path); }
    void setParent(Path *p)
        { m_parent = p; }
    Path *parent()
        { return m_parent; }
    const std::vector<Path>& subPaths() const
        { return m_children; }
    const std::vector<Point> points() const
    {
        //ABELL
        return std::vector<Point>();
    }
    const HexId rootHex() const
        { return m_segments[0].hex; }

private:
    /// Parent path (NULL if root)
    Path *m_parent;
    /// Children
    std::vector<Path> m_children;
    /// Orientation of path AT EXTRACTION - segments are ALWAYS ordered
    /// clockwise.
    Orientation m_orientation;
    /// List of segments that make up the path.
    std::vector<Segment> m_segments;
    /// ABELL - ?
    int m_pathNum;
};

} //namespace hexer