import { GmodPath } from '../GmodPath';

export interface PathNode {
  code: string;
  location?: string;
}

export interface ParseContext {
  parts: PathNode[];
  toFind: PathNode;
  locations: Map<string, string>;
  path?: GmodPath;
}
